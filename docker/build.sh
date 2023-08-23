#!/bin/sh
set -eu
docker build --rm \
    -f ./docker/Dockerfile \
    -t jrmsdev/linnetfs \
    --build-arg LNFS_UID=$(id -u) \
    ./docker
exit 0
