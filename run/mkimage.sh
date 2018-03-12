#!/usr/bin/env bash
ORIG=$(pwd)                                                                                                       
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           
ROOT="$(dirname $DIR)"
PROJ="$ROOT/hello-cpp-linwin/"
STAGE="$ROOT/stage/"

indent(){
    sed 's/^/    /'
}

echo "Building build image (required for run)"
"$DIR/../build/mkimage.sh" | indent

printf "\nExtracting binaries from $USER/hellobuild:latest\n"
# start a container on the build image
build_container="$(docker run -d $USER/hellobuild:latest /bin/bash)"

# copy the binary from the container
docker cp "$build_container:/usr/local/src/hello/build/bin/hello" "$DIR/hello"

# clean up the container
docker rm "$build_container" 1> /dev/null

printf "\n"

cd "$DIR"
docker build . -t "$USER/hellorun:latest" | indent

printf "\ncleaning up...
\n"
rm "$DIR/hello"

echo "Done building 'build'"
echo "to poke around the run image, run:"
echo "docker run --rm -it $USER/hellorun:latest /bin/bash"

cd "$ORIG"
