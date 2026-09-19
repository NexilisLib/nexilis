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

import os
import subprocess
from pathlib import Path
from typing import Optional, Union, List


def run_command(
    command: Union[str, List[str]],
    cwd: Optional[Union[str, Path]] = None,
    env: Optional[dict] = None,
    shell: bool = True,
    quiet: bool = False,
    check: bool = True,
) -> subprocess.CompletedProcess:
    """
    Run a shell command with optional working directory and environment variables.

    Args:
        command: The command to run (as string or list of args).
        cwd: Working directory to run the command in.
        env: Additional environment variables.
        shell: Whether to run through shell.
        quiet: If True, don't print command info.
        check: If True, raise CalledProcessError on non-zero exit code.

    Returns:
        CompletedProcess object with returncode, stdout, stderr.
    """
    if not quiet:
        cmd_str = command if isinstance(command, str) else " ".join(command)
        print(f"→ Running: {cmd_str}")
        if cwd:
            print(f"  in directory: {cwd}")

    # Convert Path objects to strings if needed.
    if isinstance(cwd, Path):
        cwd = str(cwd)

    # Merge with current environment if needed.
    process_env = os.environ.copy()
    if env:
        process_env.update(env)

    return subprocess.run(
        command,
        cwd=cwd,
        env=process_env,
        shell=shell,
        check=check,
        text=True,
        errors="replace",
        capture_output=True,
    )
