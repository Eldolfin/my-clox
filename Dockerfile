FROM ubuntu:22.04
LABEL Description="Build environment"

ENV HOME /root
ENV DEBIAN_FRONTEND noninteractive 

SHELL ["/bin/bash", "-c"]

RUN apt-get -qq update 

RUN apt-get -qq -y --no-install-recommends install \
  build-essential \
  gcc \
  make \
  gpg-agent \
  python3.10-venv

RUN python3 -m venv /venv
RUN /venv/bin/pip install unittest-xml-reporting

CMD make -C /clox clox;/venv/bin/python /clox/tests/tests.py 
