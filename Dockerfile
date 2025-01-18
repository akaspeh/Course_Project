FROM ubuntu:latest


RUN apt-get update

RUN apt-get install -y \
    build-essential \
    cmake \
    ca-certificates \
    g++-11

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 110 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 110

RUN g++ --version

WORKDIR /app

COPY src /app/src
COPY CMakeLists.txt /app/CMakeLists.txt

RUN mkdir build

RUN cmake -DCMAKE_CXX_COMPILER=g++-11 -DCMAKE_CXX_STANDARD=23 -S . -B build

RUN cmake --build build

ENTRYPOINT ["./build/Course_project"]