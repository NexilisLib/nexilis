#!/bin/bash

rebuild() {
    rm -rf build
    mkdir build
    cd build
    cmake ..
    make -j9
    cd ..
}

main() {
    cd ../nexilis/
    rebuild
    cd ../server/chat-server/
    rebuild
    cd ../../client/chat-client/
    rebuild
}

main
