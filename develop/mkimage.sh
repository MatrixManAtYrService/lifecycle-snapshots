#!/usr/bin/env bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           
docker build "$DIR" -t "$USER/hellodev:latest"

echo
echo "Done building 'run'"
echo "to poke around the image, run:"
echo "docker run --rm -it $USER/hellodev:latest /bin/bash"
