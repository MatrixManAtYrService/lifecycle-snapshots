# Lifecycle Snapshots

This repo is a template for using docker to build software in containers.  It builds [a fancy hello world implementation](https://github.com/MatrixManAtYrService/hello-cpp-linwin).  The build proceeds in four lifecycle snapshots:
- Run
- Develop
- Build
- Deploy

Ultimately, an image is created for each of these.  There is no reason that you should pick four snapshots (how about some for tests?), but that's how this example works.

### Run
This encapsulates anything that you expect to already be installed on the user's machine.  If you refer somebody to [run/Dockerfile](run/Dockerfile) they can see what your run environment expectations are.

### Develop 
This houses the code and any tools you rely on for development.  New developers can look at [develop/Dockerfile](develop/Dockerfile) to get ideas about how they should configure their development environments--or they can develop right in the container.

### Build
When docker builds [build/Dockerfile](build/Dockerfile) this image, it executes your build.  Anything that relies on your build outputs can execute in containers based on this image

### Deploy
This is what you should push to dockerhub so that users can run your software without worrying about dependencies (except for docker itself).  The way to run your software is documented at the end of [deploy/Dockerfile](deploy/Dockerfile).

# Running it

### Docker

You'll need docker installed (or running elsewhere) on something unixy.  Windows users ought to be able to follow this pattern, but they can't base their 'run' image on debian like I have.

### Submodules

This repo contains a submodule.  Its submodule also contains a submodule.  To resolve them, run this after cloning it:

    git submodule update --init --recursive

### Making Snapshots

Then, try these commands:

    ./run/snap.sh
    ./develop/snap.sh
    ./build/snap.sh
    ./deploy/snap.sh

# Things to notice

## Hints
Each 'snap.sh' script produces output like this:

    To explore 'run' use:
    docker run --rm -it hellorun /bin/bash

Try running those commands to interact with your built images

## Reruns are fast

Docker looks at the inputs to a build to determine wheter it needs to update the image.  So just because 'run' takes a while the first time (it's downloading and installing stuff) doesn't mean it will take that long later.  Unless you change something about the runtime environment

## The snap scripts call each other

'develop' depends on 'run', so it calls 'run' before it builds a snapshot.  This ensures that the run snapshot is available for 'develop' to layer on.

This kind of dependency management is what `make` is for, there's probably a more elegant way.  When I become a `make` wizard, I'll implement it here.

## Container and image storage
Try these commands:

    docker images
    docker ps -a

They will show you docker stuff that these scripts will leave sitting around on your machine.  If you play with docker often, you should consider cleaning this stuff up every now and then.




