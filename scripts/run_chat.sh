#!/bin/bash

# Function to start a screen session with a given name and command.
start_screen_session() {
    local session_name=$1
    local command=$2
    screen -dmS "$session_name" bash -c "$command"
}

# Function to run a binary in a screen session.
run_binary_in_screen() {
    local binary=$1
    local session_name=$2
    start_screen_session "$session_name" "$binary"
}

# Build the chat.
./build_chat.sh

# Run the server binary in a screen session named "server_session".
run_binary_in_screen "../server/chat-server/build/chat_server" "server_session"

# Run the client binary in a screen session named "client_session"
run_binary_in_screen "../client/chat-client/build/chat_client" "client_session"
