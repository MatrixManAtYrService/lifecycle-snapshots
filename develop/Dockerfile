# This dockerfile makes a snapshot of the development environment
FROM hellorun

# install build dependencies
RUN apt-get install -y \
apt-utils \
git \
build-essential \
make \
libboost-test-dev \
libboost-program-options1.65-dev

# cmake can't find boost 1.65 unless it is at 3.9.3 or newer
# see: https://stackoverflow.com/questions/42123509/cmake-finds-boost-but-the-imported-targets-not-available-for-boost-version
# and: https://askubuntu.com/questions/952429/is-there-a-good-ppa-for-cmake-backports
RUN apt-get install -y python-pip && \
        pip install --upgrade cmake

# include the code
COPY hello-cpp-linwin/ /usr/local/src/hello

# remove intermediate files (to ensure a clean build later)
RUN rm -rf /usr/local/src/hello/build

# since hello-cpp-linwin is a submodule on the local machine
# git relocated .git/ to the parent repo, un-submodule it in the image
RUN rm /usr/local/src/hello/.git
COPY .git/modules/hello-cpp-linwin /usr/local/src/hello/.git/


