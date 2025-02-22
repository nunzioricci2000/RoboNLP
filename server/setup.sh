#!/bin/bash

echo "Setting up the environment for the project"

if [[ " $@ " == *" --avoid-download "* ]]; then
    echo "Skipping dependency download..."
else
    echo -n "Downloading dependencies..."
    git submodule update --init --recursive > /dev/null
    echo "done"
fi

mkdir -p libs

echo -n "Installing dependencies..."
cp external/cJSON/cJSON.c libs/cJSON.c
cp external/cJSON/cJSON.h libs/cJSON.h
cp external/picohttpparser/picohttpparser.c libs/picohttpparser.c
cp external/picohttpparser/picohttpparser.h libs/picohttpparser.h
echo "done"

echo "Environment setup complete!"