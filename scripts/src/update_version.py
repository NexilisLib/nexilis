from env import get_nexilis_root

import re
import sys
import argparse
from pathlib import Path

# Version pattern: {MAJOR}.{MINOR}.{PATCH}-{stable|unstable}
VERSION_PATTERN = r"^(\d+)\.(\d+)\.(\d+)-(stable|unstable)$"

def validate_version(version):
    """Strictly validate version format."""
    match = re.fullmatch(VERSION_PATTERN, version)
    if not match:
        raise ValueError(
            f"Invalid version: '{version}'\n"
            f"Required format: MAJOR.MINOR.PATCH-(stable|unstable)\n"
            f"Examples: '1.0.0-stable', '0.1.3-unstable'"
        )
    return version

def update_version_file(version):
    """Update VERSION.txt."""
    version_file = "VERSION.txt"
    path = Path(get_nexilis_root()) / version_file
    path.write_text(version)
    print(f"Updated {version_file} -> {version}")

def update_doxyfile(version):
    """Update PROJECT_NUMBER in Doxyfile."""
    doxyfile = "Doxyfile"
    doxyfile_path = Path(get_nexilis_root()).joinpath("nexilis", doxyfile)
    content = doxyfile_path.read_text()

    updated = re.sub(
        r'^(PROJECT_NUMBER\s*=\s*).*$',
        f'\\g<1>"{version}"',
        content,
        flags=re.MULTILINE
    )

    doxyfile_path.write_text(updated)
    print(f"Updated {doxyfile} PROJECT_NUMBER -> {version}")

def main():
    parser = argparse.ArgumentParser(description="Update Nexilis version")
    parser.add_argument(
        "version",
        help="Version string in format X.Y.Z-TYPE (e.g '0.0.1-unstable)"
    )
    args = parser.parse_args()

    if not args.version:
        parser.print_help()
        print("\nError: Version argument cannot be empty", file=sys.stderr)
        sys.exit(1)

    try:
        validate_version(args.version)
        update_version_file(args.version)
        update_doxyfile(args.version)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
