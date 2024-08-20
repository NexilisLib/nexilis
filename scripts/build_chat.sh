#!/bin/bash

compile() {
    if [ -d "build" ]; then
        cd build
        make -j$(nproc)
        cd ..
    else
        mkdir build
        cd build
        cmake ..
        make -j$(nproc)
        cd ..
    fi
}

main() {
    cd ../server/chat-server/
    compile
    cd ../../client/chat-client/
    compile
}

main
