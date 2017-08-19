#!/usr/bin/env bash
ORIG=$(pwd)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

args=$1

function MkBuildDir 
{
    # create a build directory and enter it
    mkdir -p $1
    cd $1

    # put a .gitignore file there so user-environment-specific files don't get checked in accidentally
    echo "*" > .gitignore &&
    echo "!.gitignore" >> .gitignore &&

    # leave a breadcrumb so reconfigure.sh can recreate the right environment
    echo "$1;$args" > "$DIR/.breadcrumb"

    # return to where caller was
    cd ..
}

# -g (GNU Make)
if [[ $1 =~ -g ]] ; then

    # Release build
    if [[ $1 =~ -.r ]] ; then

        BUILD_DIR="$DIR/release"
        MkBuildDir $BUILD_DIR

        cmake -H$DIR -B$BUILD_DIR -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release

    # Debug build
    else
        BUILD_DIR="${DIR}/debug"
        MkBuildDir $BUILD_DIR

        cmake -H$DIR -B$BUILD_DIR -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
    fi

# -v (Visual Studio)
elif [[ $1 =~ -v ]] ; then

    # Both builds
    BUILD_DIR="${DIR}/build"
    MkBuildDir $BUILD_DIR

    cmake -H$DIR -B$BUILD_DIR -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -G "Visual Studio 14 2015 Win64"

# print help message
else
    echo "Generates a build configuration using CMake and puts it in ./build ./debug or ./release"
    echo "These project files are generated--so changes to them might get clobbered."
    echo ""
    echo "If you need to make a persistent change, consider modifying CMakeLists.txt"
    echo "That way, your change will propagate into everybody's configuration--not just yours."
    echo ""
    echo "Usage: configure [OPTION]"
    echo ""
    echo  "<none>                   Display this message"
    echo  "-g                       Configure debug build directory for gnu make (./debug)"
    echo  "-gr                      Configure release build directory for gnu make (./release)"
    echo  "-v                       Configure build directory for Visual Studio 2015 (Win64) (./build)"

fi

# return to original directory
cd $ORIG
