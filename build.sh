#!/bin/bash

IMAGE_NAME=muenvsubst-build

docker build --progress plain --tag ${IMAGE_NAME} .

id=$(docker create ${IMAGE_NAME})
mkdir -p dist
docker cp $id:/build/muenvsubst ./dist/
docker rm -v $id

exit 1

set -e
set -x

BASEDIR=$(dirname "$0")
pushd "$BASEDIR"

rm -rf build

conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
