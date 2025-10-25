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
    check: bool = True
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
        capture_output=True
    )
