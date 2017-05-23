#!/usr/bin/env bash
ORIG=$(pwd)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# search for breadcrumbs, use the first one if multiple found
if [[ ! -a $DIR/.breadcrumb ]] ; then

    echo "No breadcrumb found, run configure.sh"
    exit 1
else
    # read breadcrumb
    CRUMB=$(cat "$DIR/.breadcrumb")
    IFS=';' read -ra C_ARR <<< "$CRUMB"

    # extract the config params and build dir
    BUILD_DIR=${C_ARR[0]}
    ARG=${C_ARR[1]}

    # remove the build dir and re-call configure
    rm -rf $BUILD_DIR
    ./configure.sh "$ARG"
fi

