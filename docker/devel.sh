#!/bin/sh
set -eu
install -d -m 0750 ./build
docker run --rm -it --read-only \
	--name linnetfs-devel \
	--hostname devel.linnetfs.local \
	-v "${PWD}:/home/lnfs/src:ro" \
	-v "${PWD}/build:/home/lnfs/src/build" \
	--tmpfs /tmp \
	jrmsdev/linnetfs
exit 0
