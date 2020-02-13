FROM ubuntu:18.04
RUN apt update && \
    apt install -y software-properties-common && \
    add-apt-repository ppa:beineri/opt-qt-5.14.1-bionic && \
    apt update && \
    apt upgrade -y && \
    apt install -y \
        cmake \
        libpng-dev \
        uuid \
        libgl-dev \
        libglew-dev \
        libglu-dev \
        libglfw3-dev \
        libgdal-dev \
        qt514-meta-full \
        git

WORKDIR /build
RUN cd /build && \
    git clone https://gitlab.com/CBDS/MapTiler.git

RUN mkdir /build/MapTiler/build && \
    cd /build/MapTiler/build && \
    cmake ..
RUN cd /build/MapTiler/build && make clean && make


