FROM hellodev

RUN apt-get clean && apt-get update

RUN apt-get install -y \
libboost-program-options1.58.0

COPY hello*.deb /root/

RUN apt-get install /root/hello*.deb

ENTRYPOINT ["hello"]
