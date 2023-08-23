#!/bin/sh
set -eu
docker run --rm -it \
    --name linnetfs-devel \
    --hostname devel.linnetfs.local \
    -v "${PWD}:/home/lnfs/src" \
    jrmsdev/linnetfs
exit 0
