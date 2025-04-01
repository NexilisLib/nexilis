#!/bin/bash
set -e

cd ../../nexilis
make clean
premake5 gmake2
make config=debug -j$(nproc)

cd ../tests/premake
make clean
premake5 gmake2
make config=debug -j$(nproc)

cd bin/Debug
LD_LIBRARY_PATH=. ./premake_test

echo "=== Test Completed Successfully ==="
