#!/usr/bin/env bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           
ROOT="$(realpath $DIR/..)"
PROJ="$ROOT/hello-cpp-linwin/"

indent(){
    sed 's/^/    /'
}

echo "Taking a 'run' snapshot first (required for 'develop')"
"$DIR/../run/snap.sh" | indent

echo "Taking snapshot: 'develop'"

# If this is a build server, you might do something like this here:
##build the latest version, not the current version
# cd $PROJ
# git clean -df
# git reset --hard
# git pull origin master

# copy the dockerfile to the project root so it can be a parent of the source
# (necessary because docker hashes children to see if rebuilding a layer is needed)
cp -p "$DIR/Dockerfile" "$ROOT"

cd "$ROOT"
docker build . -t "hellodev:latest"

rm "$ROOT/Dockerfile"

echo "To explore 'develop' run:"
echo "docker run --rm -it hellodev /bin/bash"
echo
