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
wget \
git

# Minishell tester
RUN bash -c "$(curl -fsSL https://raw.githubusercontent.com/zstenger93/42_minishell_tester/master/install.sh)"

# Copy Readline lib to 'local' dir
RUN cp -R /usr/include/readline /usr/local/include/readline

# LLDB: disable aslr
RUN echo "settings set target.disable-aslr false" >> ~/.lldbinit

# Create user 'user'
RUN useradd -m -s /bin/bash user \
&& usermod -aG sudo user \
&& echo 'user ALL=(ALL) NOPASSWD: ALL' > /etc/sudoers.d/user

# Install Bash 3.2
RUN wget http://ftp.gnu.org/gnu/bash/bash-3.2.tar.gz \
&& tar -xzf bash-3.2.tar.gz \
&& cd bash-3.2 \
&& ./configure --prefix=/usr \
&& make && make install \
&& cd .. && rm -rf bash-3.2*

USER user