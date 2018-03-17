# This file creates a container for deployment, it starts with the runtime environment
FROM hellorun

# Copies the installer
COPY hello*.deb /root

# Installs it
RUN apt-get install -y /root/hello*.deb

# And configures containers to call the installed program
ENTRYPOINT ["hello"]
