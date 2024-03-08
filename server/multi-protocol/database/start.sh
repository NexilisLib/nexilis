#!/bin/bash

# Run with sudo or use docker properly.
# Nexilis is intended to work with all mariadb servers.
# This is just an example script.
#
# Manual enter to database:
# mysql -u root -p -h localhost -P 3306

CONTAINER_NAME="example-database"

# Remove existing container if it exists
if [ "$(sudo docker ps -a -q -f name="$CONTAINER_NAME")" ]; then
    docker stop "$CONTAINER_NAME"
    docker rm "$CONTAINER_NAME"
fi

# Remove existing Docker image if it exists
docker rmi -f "$CONTAINER_NAME" >/dev/null 2>&1

# Build Docker image
docker build -t "$CONTAINER_NAME" .

# Run Docker container in detached mode
docker run -d --name "$CONTAINER_NAME" -p 3306:3306 "$CONTAINER_NAME"

# Start a new screen session for running the container
screen -S database-session -d -m docker logs -f "$CONTAINER_NAME"
