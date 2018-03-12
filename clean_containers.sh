#!/usr/bin/env bash

for image in "$(docker images | egrep "hello(run|dev|build)?")" ; do
    image_name=$(echo "$image" | awk {'print $1'})
    echo $image_name
    if [[ -z $image_name ]] ; then
        for container in "$(docker ps -a | grep $image_name)" ; do
            container_id=$(echo $container | awk {'print $1'})
            echo -n "$image_name.$container_id"
            docker rm -v "$container_id"
        done
    fi
done
