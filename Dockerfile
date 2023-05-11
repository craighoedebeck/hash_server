FROM ubuntu:12.04

RUN apt-get update && apt-get install -y \
  cmake \

WORKDIR /app

COPY . .
RUN cmake .
RUN make
CMD ["./hash_server"]
