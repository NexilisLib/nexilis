#!/bin/bash
rm -rf build
cmake -B build -DCMAKE_INSTALL_PREFIX=${PWD}/build/install
cmake --build build --target install -- -j${nproc}

