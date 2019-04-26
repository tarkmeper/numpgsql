FROM postgres:latest

RUN apt-get update
RUN apt-get install -y --force-yes build-essential wget git
RUN apt-get install -y --force-yes zlib1g-dev libssl-dev libreadline-dev libyaml-dev libxml2-dev libxslt-dev
RUN apt-get install -y --force-yes libboost-all-dev postgresql-server-dev-11 libpq-dev
RUN apt-get clean

RUN git clone https://github.com/tarkmeper/numpgsql.git /root/numpgsql
RUN cd /root/numpgsql; make -j ; make install

