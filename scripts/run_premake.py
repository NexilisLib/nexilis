import os
import subprocess
from pathlib import Path
from create_env import get_nexilis_root
from utils.run_command import run_command

def main():
    nexilis_root = Path(get_nexilis_root())
    print(f"Nexilis root: {nexilis_root}")

    try:
        # Build premake nexilis.
        nexilis_dir = nexilis_root / "nexilis"
        os.chdir(nexilis_dir)

        run_command("premake5 gmake2")
        run_command("make clean")
        run_command(f"make config=debug -j{os.cpu_count()}")

        # Build tests.
        os.chdir(nexilis_root)
        tests_dir = nexilis_root / "tests" / "premake"
        os.chdir(tests_dir)

        run_command("premake5 gmake2")
        run_command("make clean")
        run_command(f"make config=debug -j{os.cpu_count()}")

        # Run tests.
        test_bin_dir = tests_dir / "bin" / "Debug"
        result = run_command(
            "./premake_test",
            cwd=test_bin_dir,
            env={"LD_LIBRARY_PATH": str(test_bin_dir)},
            check=False
        )
        print("=== Test Completed Successfully ===")

    except subprocess.CalledProcessError as e:
        print(f"Error: Command failed with exit code {e.returncode}")
        exit(1)

if __name__ == "__main__":
    main()
