FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /project
RUN apt-get update && apt-get install -y \
valgrind \
libreadline-dev \
nano \
build-essential \
&& cp -R /usr/include/readline /usr/local/include/readline