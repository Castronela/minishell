FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /minishell
RUN apt-get update && apt-get install -y \
valgrind \
libreadline-dev \
nano \
build-essential