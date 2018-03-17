# This dockerfile executes the build
FROM hellodev

RUN BUILD_DIR=/usr/local/src/hello/build  \
&& mkdir $BUILD_DIR \ 
&& cd $BUILD_DIR \
&& cmake .. \
&& make package \
&& mv hello*.deb hello.deb
