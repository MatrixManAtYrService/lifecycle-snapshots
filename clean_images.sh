#!/usr/bin/env bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           

"$DIR/clean_containers.sh"

docker images | egrep "$USER/hello" | awk '{print $3}' | xargs docker rmi
