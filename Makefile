# Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com>
# See LICENSE file.

LNFS_NAME ?= linnetfs

SRCD := $(PWD)
BUILDD := $(SRCD)/build

LNFS_LIB   := $(BUILDD)/lib$(LNFS_NAME).a
LNFS_MOUNT := $(BUILDD)/$(LNFS_NAME)-mount.bin

CXX_EXTRA_FLAGS ?=

CXX_FLAGS := -std=c++23 -Wall -pedantic -O2 -fPIC -pie
CXX_FLAGS += -I/usr/include/fuse3 -I$(SRCD)/include
CXX_FLAGS += -I$(BUILDD)/include
CXX_FLAGS += $(CXX_EXTRA_FLAGS)

CXX_LIBS := -lfuse3 -lpthread

.PHONY: default
default: build

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: clean
clean:
	@rm -vrf ./build/*.* ./build/include

.PHONY: build
build:
	@$(MAKE) $(LNFS_MOUNT)

.PHONY: debug
debug:
	@$(MAKE) build CXX_EXTRA_FLAGS=-DLNFS_DEBUG

# lnfs_log.o

$(BUILDD)/lnfs_log.o: $(SRCD)/include/lnfs_log.h $(SRCD)/lib/lnfs_log.cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_log.o -c $(SRCD)/lib/lnfs_log.cpp

# liblnfs

$(BUILDD)/include/lnfs_build.h:
	@mkdir -vp $(BUILDD)/include
	@echo "#define LNFS_NAME $(LNFS_NAME)" >$(BUILDD)/include/lnfs_build.h

LNFS_LIB_OBJ := $(BUILDD)/lnfs_log.o

$(LNFS_LIB): $(SRCD)/include/lnfs.h $(BUILDD)/include/lnfs_build.h $(LNFS_LIB_OBJ)
	@rm -vf $(LNFS_LIB)
	@$(AR) rv $(LNFS_LIB) $(LNFS_LIB_OBJ)

# lnfs-mount

$(LNFS_MOUNT): $(LNFS_LIB) $(SRCD)/bin/lnfs-mount/mount-main.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_MOUNT) $(SRCD)/bin/lnfs-mount/mount-main.cpp $(LNFS_LIB) $(CXX_LIBS)
