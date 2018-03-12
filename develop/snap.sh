#!/usr/bin/env bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"                                                           
ROOT="$(realpath $DIR/..)"
STAGE="$ROOT/stage/"
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

printf "Staging $PROJ for 'develop', copying files:\n    "

# A place to put code that is going into the build image.
# (we want to include only files git knows about,
# this prevents developers from accidentally depending on local-only files)
mkdir -p "$STAGE"
cp -p "$DIR/Dockerfile" "$ROOT"

cd "$PROJ"
for file in $(git ls-tree --full-tree --name-only -r HEAD); do

    # If your repo contains large files not strictly necessary for the build,
    # you can filter them here
    if [[ ! "$file" =~ .md ]] ; then
        cp -r --parent "$file" "$STAGE"
        echo -n "$(basename $file), "
    fi
done
printf "\n"

# It is somewhat wasteful to include the whole git folder (history and all),
# but not doing so breaks the git version stamp.
# If this becomes a bottleneck, consider doing the version stamp here and 
# modifying the project so that it simply picks up the version stamp from a file
cp -r --parent .git "$STAGE"

cd "$ROOT"
docker build . -t "hellodev:latest"

echo "cleaning up..."
rm "$ROOT/Dockerfile"
rm -rf "$STAGE"

echo "To explore 'develop' run:"
echo "docker run --rm -it hellodev /bin/bash"
echo
