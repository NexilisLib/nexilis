#!/usr/bin/env python3

import argparse
import os
from dotenv import load_dotenv


def _get_project_root_path():
    return os.path.abspath(os.path.dirname(os.path.dirname(os.path.dirname(__file__))))


def get_nexilis_root(env_dir: str = None):
    if env_dir is None:
        env_dir = _get_project_root_path()
    load_dotenv(os.path.join(env_dir, ".env"))

    nexilis_root = os.getenv("NEXILIS_ROOT")
    if not nexilis_root:
        print("Warning: NEXILIS_ROOT not found in .env file. Using absolute path.")
        nexilis_root = _get_project_root_path()

    return nexilis_root


def create_env_file(output_dir: str = None):
    nexilis_root = _get_project_root_path()
    if output_dir is None:
        output_dir = nexilis_root
    output_dir = os.path.abspath(output_dir)

    env_file_path = os.path.join(output_dir, ".env")

    with open(env_file_path, "w") as env_file:
        env_file.write(f"NEXILIS_ROOT={nexilis_root}\n")
    print(f".env file created at {env_file_path}")


def main():
    parser = argparse.ArgumentParser(description="Create .env file with NEXILIS_ROOT")
    parser.add_argument(
        "-o", "--output",
        type=str,
        default=None,
        help="Directory to create .env file in (default: nexilis root)"
    )
    args = parser.parse_args()

    create_env_file(args.output)

    path = get_nexilis_root(args.output)
    print(f"Nexilis root: {path}")


if __name__ == "__main__":
    main()
