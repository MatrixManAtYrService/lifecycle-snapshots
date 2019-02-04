# This dockerfile executes the build, it starts from the dev environment
FROM hellodev

ENV DEBIAN_FRONTEND noninteractive

# These are the build steps
RUN BUILD_DIR=/usr/local/src/hello/build  \
&& mkdir $BUILD_DIR \ 
&& cd $BUILD_DIR \
&& cmake .. \
&& make package \
&& mv hello*.deb hello.deb
