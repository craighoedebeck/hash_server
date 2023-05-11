FROM ubuntu:20.04

WORKDIR /app

COPY . .

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Etc/UTC
RUN apt-get update && apt-get install -y \
  cmake \
  build-essential \
  libboost-all-dev \
  libssl-dev \
  libgtest-dev

RUN cmake .
RUN make
