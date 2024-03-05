#!/bin/bash

IMAGE_NAME=muenvsubst-build

docker build --progress plain --tag ${IMAGE_NAME} -f Dockerfile.musl .

id=$(docker create ${IMAGE_NAME})
mkdir -p dist
docker cp $id:/build/muenvsubst ./dist/
docker rm -v $id
