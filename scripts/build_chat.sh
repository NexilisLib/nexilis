#!/bin/bash

compile() {
    if [ -d "build" ]; then
        cd build
        make -j9
        cd ..
    else
        mkdir build
        cd build
        cmake ..
        make -j9
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
