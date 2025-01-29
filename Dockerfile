FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
WORKDIR /project
RUN apt-get update && apt-get install -y \
valgrind \
libreadline-dev \
nano \
lldb \
build-essential \
sudo \
curl \
ncurses-base \
git ;\
bash -c "$(curl -fsSL https://raw.githubusercontent.com/zstenger93/42_minishell_tester/master/install.sh)";  \
cp -R /usr/include/readline /usr/local/include/readline; \
echo "settings set target.disable-aslr false" >> ~/.lldbinit; \
useradd -m -s /bin/bash user \
&& usermod -aG sudo user \
&& echo 'user ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/user \