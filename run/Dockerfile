FROM ubuntu:16.04

RUN apt-get clean && apt-get update

RUN apt-get install -y \
libboost-program-options1.58.0

COPY hello /usr/local/bin/hello

ENTRYPOINT hello
