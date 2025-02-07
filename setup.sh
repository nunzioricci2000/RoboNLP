#!/bin/bash

echo "Setting up the environment for the project"

echo -n "Downloading dependencies..."
git submodule update --init --recursive > /dev/null
echo "done"

mkdir -p server/libs

echo -n "Installing dependencies..."
cp server/external/cJSON/cJSON.c server/libs/cJSON.c
cp server/external/cJSON/cJSON.h server/libs/cJSON.h
cp server/external/picohttpparser/picohttpparser.c server/libs/picohttpparser.c
cp server/external/picohttpparser/picohttpparser.h server/libs/picohttpparser.h
echo "done"

echo "Environment setup complete!"