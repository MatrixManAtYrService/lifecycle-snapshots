# This dockerfile makes a snapshot of the development environment
FROM hellorun

# install build dependencies
RUN apt-get install -y \
apt-utils \
git \
build-essential \
make \
cmake \
libboost-test-dev \
libboost-program-options1.58-dev

# include the code
COPY hello-cpp-linwin/ /usr/local/src/hello

# remove intermediate files (to ensure a clean build later)
RUN rm -rf /usr/local/src/hello/build

# since hello-cpp-linwin is a submodule on the local machine
# git relocated .git/ to the parent repo, un-submodule it in the image
RUN rm /usr/local/src/hello/.git
COPY .git/modules/hello-cpp-linwin /usr/local/src/hello/.git/


