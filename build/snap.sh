#!/usr/bin/env bash
ORIG=$(pwd)                                                                                                       
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           
ROOT="$(dirname $DIR)"
PROJ="$ROOT/hello-cpp-linwin/"
STAGE="$ROOT/stage/"

indent(){
    sed 's/^/    /'
}

echo "Taking a 'develop' snapshot first (required for 'build')"
"$DIR/../develop/snap.sh" | indent

cd "$DIR"
docker build . -t "hellobuild:latest"

echo "things to try:"
echo "docker run --rm -it hellobuild /bin/bash"
echo "docker run --rm hellobuild /usr/local/src/hello/build/testBin/cow_test --log_level=test_suite"
echo "docker run --rm hellobuild /usr/local/src/hello/build/bin/hello"
echo "docker run --rm hellobuild /usr/local/src/hello/build/bin/hello --version"
echo

cd "$ORIG"
