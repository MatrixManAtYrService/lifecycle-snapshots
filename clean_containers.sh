#!/usr/bin/env bash

for image in "$(docker images | egrep "$USER/hello")" ; do
    image_name=$(echo "$image" | awk {'print $1'})
    if [[ -z $image_name ]] ; then
        for container in "$(docker ps -a | grep $image_name)" ; do
            container_id=$(echo $container | awk {'print $1'})
            docker rm -v "$container_id"
        done
    fi
done
