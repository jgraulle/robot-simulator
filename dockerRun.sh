#!/bin/bash

if [ "$1" == "build" ]
then
    # Build image
    docker build \
        --tag robot \
        --build-arg USER_NAME=${USER} \
        --build-arg USER_ID=$(id -u ${USER}) \
        --build-arg GROUP_ID=$(id -g ${USER}) \
        .
elif [ "$1" == "run" ]
then
    # Create vscode folder to avoid default creation with root user
    mkdir -p .vscode-server

    # On wayland we need this
    xhost +SI:localuser:$(id -un)

    # Run image
    docker run --interactive --tty --rm \
        --env DISPLAY=${DISPLAY} \
        --volume /tmp/.X11-unix:/tmp/.X11-unix \
        --volume ${PWD}/.vscode-server:/home/${USER}/.vscode-server \
        --volume $(pwd):$(pwd) \
        --workdir $(pwd) \
        robot
else
    echo 'You must give "build" or "run" as argument'
fi
