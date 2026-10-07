#     Copyright (C) 2026 Valtteri Viirret
#     This file is part of the Nexilis Project.
#
#     This file is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
#
#     This file is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public License
#     along with this file.  If not, see <https://gnu.org>.

from pre_commit.env import get_nexilis_root

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


def increment_patch(version):
    """Increment the patch number while preserving the release status."""
    validate_version(version)
    major, minor, patch, status = re.fullmatch(VERSION_PATTERN, version).groups()
    return f"{major}.{minor}.{int(patch) + 1}-{status}"


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
        r"^(PROJECT_NUMBER\s*=\s*).*$",
        f'\\g<1>"{version}"',
        content,
        flags=re.MULTILINE,
    )

    doxyfile_path.write_text(updated)
    print(f"Updated {doxyfile} PROJECT_NUMBER -> {version}")


def main():
    parser = argparse.ArgumentParser(description="Update Nexilis version")
    parser.add_argument(
        "version", nargs="?",
        help="Version string in format X.Y.Z-TYPE (e.g '0.0.1-unstable')",
    )
    parser.add_argument(
        "--bump-patch", action="store_true",
        help="Increment the patch number in VERSION.txt, preserving stable/unstable",
    )
    args = parser.parse_args()

    if bool(args.version) == args.bump_patch:
        parser.error("provide either a version or --bump-patch")

    try:
        version = args.version
        if args.bump_patch:
            current = (Path(get_nexilis_root()) / "VERSION.txt").read_text().strip()
            version = increment_patch(current)
        validate_version(version)
        update_version_file(version)
        update_doxyfile(version)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
