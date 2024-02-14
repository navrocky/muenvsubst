FROM alpine:3.19.1

RUN set -x && \
    apk add cmake py3-pip g++ make && \
    pip install conan --break-system-packages

RUN conan profile detect --force

COPY conanfile.txt /sources/

RUN set -x && \
    mkdir -p /build && \
    cd /build && \
    conan install /sources --output-folder=. --build=missing

COPY . /sources/

RUN set -x && \
    cd /build && \
    cmake /sources -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release && \
    cmake --build .
