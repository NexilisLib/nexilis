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

from env import get_nexilis_root

import os
import subprocess
import sys


def format_all_files() -> int:
    # Define the directories to run Clang-Format recursively.
    directories = ["nexilis", "examples", "tests"]
    changes_detected = False

    # Run Clang-Format recursively in the specified directories.
    for directory in directories:
        target_dir = os.path.join(get_nexilis_root(), directory)
        if not os.path.isdir(target_dir):
            continue

        print(f"Formatting files in directory: {target_dir}")

        # Check if files need formatting.
        find_check_command = (
            rf'find "{target_dir}"'
            r' -not -path "*/third-party/*"'
            r' -type f \( -name "*.hh" -o -name "*.cc" \)'
            r" -exec clang-format --dry-run --Werror {} +"
        )
        result = subprocess.run(
            find_check_command, shell=True, capture_output=True, text=True
        )

        if result.returncode != 0:
            print("::error::Formatting changes required. Diff:")
            print(result.stderr)
            changes_detected = True

            # Actually apply formatting.
            find_apply_command = (
                rf'find "{target_dir}"'
                r' -not -path "*/third-party/*"'
                r' -type f \( -name "*.hh" -o -name "*.cc" \)'
                r" -exec clang-format -i {} +"
            )
            subprocess.run(find_apply_command, shell=True, check=True)

    if changes_detected:
        print("::error::Formatting changes were required and have been applied")
        return 1
    else:
        print("No formatting changes needed")
        return 0


if __name__ == "__main__":
    f = format_all_files()
    sys.exit(f)
