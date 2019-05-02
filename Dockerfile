FROM postgres:latest

RUN apt-get update
RUN apt-get install -y --force-yes build-essential wget git systemd lcov curl
RUN apt-get install -y --force-yes zlib1g-dev libssl-dev libreadline-dev libyaml-dev libxml2-dev libxslt-dev
RUN apt-get install -y --force-yes libboost-math-dev libboost-random-dev postgresql-server-dev-11 libpq-dev
RUN apt-get clean

RUN whoami
RUN mkdir /build
RUN chmod a+rwx /build

USER postgres
RUN whoami
RUN /usr/lib/postgresql/11/bin/initdb
RUN git clone https://github.com/tarkmeper/numpgsql.git /build/numpgsql
RUN cd /build/numpgsql; make -j with_llvm=no ccov=yes; 
RUN cd /build/numpgsql; chmod a+wrx $(find src -type d)

USER root
RUN cd /build/numpgsql; make install with_llvm=no

