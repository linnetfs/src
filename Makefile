# Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com>
# See LICENSE file.

LNFS_NAME ?= linnetfs

SRCD := $(PWD)
BUILDD := $(SRCD)/build

LNFS_LIB   := $(BUILDD)/lib$(LNFS_NAME).o
LNFS_MOUNT := $(BUILDD)/$(LNFS_NAME)-mount.bin

CXX_FLAGS := -std=c++23 -Wall -pedantic -O2 -fPIC -I/usr/include/fuse3
CXX_FLAGS += -I$(SRCD)/include

CXX_LIBS := $(LNFS_LIB) -lfuse3 -lpthread

.PHONY: default
default: build

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: clean
clean:
	@rm -vf ./build/*.*

.PHONY: build
build:
	@$(MAKE) $(LNFS_MOUNT)

$(LNFS_LIB): $(SRCD)/include/lnfs*.h $(SRCD)/lib/lnfs*.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_LIB) -c $(SRCD)/lib/lnfs*.cpp

$(LNFS_MOUNT): $(LNFS_LIB) $(SRCD)/bin/lnfs-mount/mount-main.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_MOUNT) $(SRCD)/bin/lnfs-mount/mount-main.cpp $(CXX_LIBS)
