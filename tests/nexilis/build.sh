#!/bin/bash
mkdir -p build && cd build
cmake .. -DNEXILIS_IS_LOCAL=1
cmake --build . -j$(nproc)

