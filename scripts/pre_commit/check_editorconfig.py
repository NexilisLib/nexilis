#!/usr/bin/env python3
"""Check files against the project's .editorconfig rules."""

import os
import re
import sys
import fnmatch
import argparse
import subprocess


def expand_braces(pattern):
    """Expand {a,b,c} alternatives into a list of patterns."""
    match = re.search(r"\{([^{}]*)\}", pattern)
    if not match:
        return [pattern]
    options = match.group(1).split(",")
    result = []
    for opt in options:
        new_pat = pattern[: match.start()] + opt.strip() + pattern[match.end():]
        result.extend(expand_braces(new_pat))
    return result


def matches_pattern(pattern, rel_path):
    """Check if a relative file path matches an editorconfig glob pattern."""
    filename = os.path.basename(rel_path)
    for p in expand_braces(pattern):
        if "/" in p:
            if fnmatch.fnmatch(rel_path, p):
                return True
        else:
            # No path separator: match against filename only
            if fnmatch.fnmatch(filename, p):
                return True
    return False


def parse_editorconfig(editorconfig_path):
    """Parse .editorconfig and return list of (pattern, rules_dict) pairs."""
    sections = []
    current_pattern = None
    current_rules = {}

    with open(editorconfig_path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#") or line.startswith(";"):
                continue
            if line.startswith("[") and line.endswith("]"):
                if current_pattern is not None:
                    sections.append((current_pattern, current_rules))
                current_pattern = line[1:-1]
                current_rules = {}
            elif "=" in line and current_pattern is not None:
                key, _, value = line.partition("=")
                current_rules[key.strip().lower()] = value.strip().lower()

    if current_pattern is not None:
        sections.append((current_pattern, current_rules))

    return sections


def get_rules_for_file(rel_path, sections):
    """Determine effective editorconfig rules for a file path.

    Later sections override earlier ones (standard editorconfig behaviour).
    The special [*] section applies to all files.
    """
    rules = {}
    for pattern, section_rules in sections:
        if pattern == "*" or matches_pattern(pattern, rel_path):
            rules.update(section_rules)
    return rules


def is_binary(data):
    """Heuristic: treat files containing null bytes as binary."""
    return b"\x00" in data


def check_file(filepath, rules):
    """Check a single file against its resolved editorconfig rules.

    Returns a list of human-readable violation strings.
    """
    violations = []

    try:
        with open(filepath, "rb") as f:
            raw = f.read()
    except OSError as e:
        return [f"cannot read file: {e}"]

    if is_binary(raw):
        return []

    # --- charset ---
    if rules.get("charset") == "utf-8":
        try:
            raw.decode("utf-8")
        except UnicodeDecodeError:
            violations.append("not valid UTF-8")
            return violations  # Text checks are meaningless after this

    # --- end_of_line ---
    eol = rules.get("end_of_line")
    if eol == "lf":
        if b"\r\n" in raw:
            violations.append("CRLF line endings (expected LF)")
        elif b"\r" in raw:
            violations.append("CR line endings (expected LF)")
    elif eol == "crlf":
        without_crlf = raw.replace(b"\r\n", b"")
        if b"\n" in without_crlf or b"\r" in without_crlf:
            violations.append("bare LF/CR line endings (expected CRLF)")

    # --- insert_final_newline ---
    insert_final = rules.get("insert_final_newline")
    if insert_final == "true":
        if raw and not raw.endswith(b"\n"):
            violations.append("missing final newline")
        elif not raw:
            violations.append("missing final newline (empty file)")
    elif insert_final == "false":
        if raw.endswith(b"\n"):
            violations.append("unexpected final newline")

    # Decode for line-level checks
    text = raw.decode("utf-8", errors="replace")
    lines = text.splitlines()

    # --- trim_trailing_whitespace ---
    if rules.get("trim_trailing_whitespace") == "true":
        for i, line in enumerate(lines, 1):
            if line != line.rstrip(" \t"):
                violations.append(f"trailing whitespace on line {i}")

    # --- indent_style ---
    indent_style = rules.get("indent_style")
    if indent_style == "space":
        for i, line in enumerate(lines, 1):
            if line.startswith("\t"):
                violations.append(f"tab indentation on line {i} (expected spaces)")
    elif indent_style == "tab":
        for i, line in enumerate(lines, 1):
            if line and line[0] == " " and line.lstrip(" "):
                violations.append(f"space indentation on line {i} (expected tabs)")

    # --- max_line_length ---
    max_len_str = rules.get("max_line_length", "off")
    if max_len_str != "off":
        try:
            max_len = int(max_len_str)
            for i, line in enumerate(lines, 1):
                length = len(line)
                if length > max_len:
                    violations.append(f"line {i} too long ({length} > {max_len})")
        except ValueError:
            pass

    return violations


def get_tracked_files(root):
    """Return absolute paths of all git-tracked files."""
    result = subprocess.run(
        ["git", "ls-files"],
        cwd=root,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        print(f"Warning: git ls-files failed: {result.stderr.strip()}")
        return []
    return [os.path.join(root, f) for f in result.stdout.splitlines()]


def find_git_root(start):
    """Walk up to find the git repository root."""
    try:
        result = subprocess.run(
            ["git", "rev-parse", "--show-toplevel"],
            cwd=start,
            capture_output=True,
            text=True,
            check=True,
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return start


def check_editorconfig(root, files):
    editorconfig_path = os.path.join(root, ".editorconfig")
    if not os.path.isfile(editorconfig_path):
        print(f"Error: no .editorconfig found at {editorconfig_path}")
        sys.exit(1)

    sections = parse_editorconfig(editorconfig_path)

    files = [os.path.abspath(f) for f in files] if files else get_tracked_files(root)

    total = 0
    for filepath in sorted(files):
        if not os.path.isfile(filepath):
            continue
        try:
            rel = os.path.relpath(filepath, root).replace(os.sep, "/")
        except ValueError:
            rel = os.path.basename(filepath)

        if rel == "VERSION.txt":
            continue

        rules = get_rules_for_file(rel, sections)
        if not rules:
            continue

        violations = check_file(filepath, rules)
        for v in violations:
            print(f"{rel}: {v}")
        total += len(violations)

    return total


def main():
    parser = argparse.ArgumentParser(
        description="Check files against .editorconfig rules."
    )
    parser.add_argument(
        "--root",
        type=str,
        default=None,
        help="Project root (default: git repo root or cwd)",
    )
    parser.add_argument(
        "files",
        nargs="*",
        help="Files to check (default: all git-tracked files)",
    )
    args = parser.parse_args()

    root = os.path.abspath(args.root) if args.root else find_git_root(os.getcwd())
    total = check_editorconfig(root, args.files)

    if total:
        print(f"\n{total} violation(s) found.")
        sys.exit(1)
    else:
        print("All files conform to .editorconfig.")


if __name__ == "__main__":
    main()
