FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /project
RUN apt-get update && apt-get install -y \
valgrind \
libreadline-dev \
nano \
lldb \
build-essential \
&& cp -R /usr/include/readline /usr/local/include/readline \
&& echo "settings set target.disable-aslr false" >> ~/.lldbinit