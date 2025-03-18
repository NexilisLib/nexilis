from create_env import get_nexilis_root

import subprocess
import sys
import os
import glob
from threading import Thread

def stream_output(stream, output_type):
    """
    Helper function to stream output from a subprocess in real-time.
    :param stream: The stream to read from (stdout or stderr).
    :param output_type: The type of output ("stdout" or "stderr").
    """
    for line in iter(stream.readline, ''):
        if output_type == "stdout":
            sys.stderr.buffer.write(line.encode())
            sys.stderr.flush()
        else:
            sys.stdout.buffer.write(line.encode())
            sys.stdout.flush()

def run_cppcheck(include_dir, src_dir, exclude_dirs=None):
    """
    Run cppcheck on the given include and source directories, excluding specified directories.

    :param include_dir: The directory containing header files.
    :param src_dir: The directory containing source files.
    :param exclude_dirs: List of directories to exclude.
    :param additional_args: Additional arguments to pass to cppcheck.
    """
    # Base cppcheck command.
    command = [
        "cppcheck",
        "--enable=all",
        "--suppress=missingIncludeSystem",
        "--inconclusive",
        "--error-exitcode=1",
        "--template=gcc",
        "--force",
        "--std=c++17",
    ]

    # Add exclude directories.
    if exclude_dirs:
        for dir in exclude_dirs:
            command.extend(["-i", dir])

    # Find all header and source files.
    header_files = glob.glob(os.path.join(include_dir, "**", "*.h"), recursive=True) + \
                   glob.glob(os.path.join(include_dir, "**", "*.hh"), recursive=True)

    source_files = glob.glob(os.path.join(src_dir, "**", "*.cc"), recursive=True) + \
                   glob.glob(os.path.join(src_dir, "**", "*.c"), recursive=True)

    # Add all source and header files to the command.
    command.extend(source_files + header_files)

    # Print the command for debugging.
    print("Running cppcheck with command:")
    print(" ".join(command), "\n")

    # Run cppcheck.
    try:
        # Start the cppcheck process.
        process = subprocess.Popen(
            ["unbuffer"] + command, # pacman -S expect
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,  # Line-buffered output
        )

        # Create threads to stream stdout and stderr in real-time.
        stdout_thread = Thread(target=stream_output, args=(process.stdout, "stdout"))
        stderr_thread = Thread(target=stream_output, args=(process.stderr, "stderr"))

        # Start the threads.
        stdout_thread.start()
        stderr_thread.start()

        # Wait for the process to complete.
        process.wait()

        # Wait for the threads to finish.
        stdout_thread.join()
        stderr_thread.join()

        # Check the return code.
        if process.returncode != 0:
            print("cppcheck failed with errors.")
            sys.exit(1)
    except Exception as e:
        print(f"An error occurred while running cppcheck: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    root = get_nexilis_root()
    include_directory = root + "/nexilis/include/nexilis/"  
    src_directory = root + "/nexilis/src/nexilis/"
    exclude_directories = [
        root + "/nexilis/include/nexilis/archived_protocols",
        root + "/nexilis/src/nexilis/archived_protocols",
    ]

    run_cppcheck(
        include_dir=include_directory,
        src_dir=src_directory,
        exclude_dirs=exclude_directories,
    )