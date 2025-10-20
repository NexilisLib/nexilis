from env import get_nexilis_root
from run_command import run_command

import sys
import os
import subprocess
from pathlib import Path


def build_and_run_premake():
    nexilis_root = Path(get_nexilis_root())
    output = [f"Nexilis root: {nexilis_root}"]

    try:
        # Build premake nexilis.
        nexilis_dir = nexilis_root / "nexilis"
        os.chdir(nexilis_dir)

        run_command("premake5 gmake2")
        run_command("make clean")
        run_command(f"make config=debug -j{os.cpu_count()}")
        output.append("Nexilis build completed successfully")

        # Build tests.
        os.chdir(nexilis_root)
        tests_dir = nexilis_root / "tests" / "premake"
        os.chdir(tests_dir)

        run_command("premake5 gmake2")
        run_command("make clean")
        run_command(f"make config=debug -j{os.cpu_count()}")
        output.append("Test build completed successfully")

        # Run tests.
        test_bin_dir = tests_dir / "bin" / "Debug"
        result = run_command(
            "./premake_test",
            cwd=test_bin_dir,
            env={"LD_LIBRARY_PATH": str(test_bin_dir)},
            check=False
        )
        output.append("=== Test Completed Successfully ===")
        return True, "\n".join(output)

    except subprocess.CalledProcessError as e:
        error_msg = f"Error: Command failed with exit code {e.returncode}"
        output.append(error_msg)
        return False, "\n".join(output)
    except Exception as e:
        error_msg = f"Unexpected error: {str(e)}"
        output.append(error_msg)
        return False, "\n".join(output)

def main():
    success, output = build_and_run_premake()
    print(output)
    sys.exit(0 if success else 1)

if __name__ == "__main__":
    main()
