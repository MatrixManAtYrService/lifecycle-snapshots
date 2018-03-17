#!/usr/bin/env bash
ORIG=$(pwd)                                                                                                       
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           

indent(){
    sed 's/^/    /'
}

echo "Taking 'build' snapshot (required for 'deploy')"
"$DIR/../build/snap.sh" | indent

printf "\nExtracting binaries from hellobuild\n"
# start a container on the build image
build_container="$(docker run -d hellobuild:latest /bin/bash)"

# copy the binary from the container
docker cp "$build_container:/usr/local/src/hello/build/hello.deb" "$DIR/"

# clean up the container
docker rm "$build_container" 1> /dev/null

printf "\n"

cd "$DIR"
docker build . -t "hello" && printf "Done building 'build'\n\n"

# clean up binary
rm "$DIR/hello.deb"

echo "Things to try:"
echo "docker run --rm -it --entrypoint=/bin/bash hello"
echo "docker run --rm hello --whale --message \"Containers are cool\""

cd "$ORIG"
