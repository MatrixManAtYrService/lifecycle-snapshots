# This dockerfile defines the expected runtime environment before the project is installed
FROM ubuntu:18.04

# Be sure to install any runtime dependencies
RUN apt-get clean && apt-get update && apt-get install -y \
libboost-program-options1.65.1 apt-utils
