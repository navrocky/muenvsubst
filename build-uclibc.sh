#!/usr/bin/env bash

IMAGE_NAME=muenvsubst-build

rm -rf ./dist/
mkdir -p ./dist/
docker buildx build --output ./dist/ --progress=plain --tag ${IMAGE_NAME} -f Dockerfile.uclibc .
