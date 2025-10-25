#!/usr/bin/env python3

import os
import sys
from dotenv import load_dotenv

def _is_venv() -> bool:
    return (
        (hasattr(sys, 'real_prefix') or
        (hasattr(sys, 'base_prefix') and sys.base_prefix != sys.prefix) or
        os.getenv("VIRTUAL_ENV") is not None)
    )

def _get_venv_root_path():
    """Get virtual environment root path"""
    if hasattr(sys, 'real_prefix'):
        print("REAL PREFIX", sys.real_prefix)
        return sys.real_prefix
    elif os.getenv('VIRTUAL_ENV'):
        return os.getenv('VIRTUAL_ENV')
    else:
        return sys.prefix


def _get_project_root_path():
    if _is_venv():
        print ("ISVENV")
        return os.path.abspath(os.path.dirname(_get_venv_root_path()))
    else:
        return os.path.abspath(os.path.dirname(os.path.dirname(os.path.dirname(__file__))))


def get_nexilis_root():
    load_dotenv(os.path.join(_get_project_root_path(), ".env"))

    nexilis_root = os.getenv("NEXILIS_ROOT")
    if not nexilis_root:
        print("Warning: NEXILIS_ROOT not found in .env file. Falling back to absolute path.")
        nexilis_root = _get_project_root_path()

    return nexilis_root


def create_env_file():
    nexilis_root = _get_project_root_path() 
    print("NEXILIS_ROOT XXXX", nexilis_root)
    env_file_path = os.path.join(nexilis_root, ".env")

    with open(env_file_path, "w") as env_file:
        env_file.write(f"NEXILIS_ROOT={nexilis_root}\n")
    print(f".env file created at {env_file_path}")


def main():
    create_env_file()

    path = get_nexilis_root()
    print(f"Nexilis root: {path}")

if __name__ == "__main__":
    main()