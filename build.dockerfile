#FROM espressif/idf:v5.3.3
FROM espressif/idf:v5.4.1

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git curl build-essential libssl-dev libreadline-dev zlib1g-dev \
    libffi-dev libyaml-dev libgdbm-dev libncurses5-dev libdb-dev \
    libgmp-dev ca-certificates

RUN git clone https://github.com/rbenv/rbenv.git ~/.rbenv && \
    cd ~/.rbenv && src/configure && make -C src && \
    git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build && \
    ~/.rbenv/plugins/ruby-build/install.sh

ENV RBENV_ROOT="/root/.rbenv"
ENV PATH="$RBENV_ROOT/bin:$RBENV_ROOT/shims:$PATH"

RUN rbenv install 3.2.8 && rbenv global 3.2.8

