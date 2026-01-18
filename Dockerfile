FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libgmp-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app/

# build
RUN mkdir -p build && cd build && \
    cmake .. && \
    make

# exe copy
RUN cp build/oasis_layer1 /usr/local/bin/ && \
    cp build/oasis_layer2 /usr/local/bin/ && \
    cp build/oasis_layer3 /usr/local/bin/ && \
    cp build/oasis_divs   /usr/local/bin/ && \
    cp build/prime_oasis  /usr/local/bin/ && \
    cp build/prime_oases  /usr/local/bin/ && \
    cp build/test_runner  /usr/local/bin/


CMD {"/bin/bash"}

