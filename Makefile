# Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com>
# See LICENSE file.

LNFS_NAME ?= linnetfs

BUILD_DIR ?= build

SRCD := $(PWD)
BUILDD := $(SRCD)/$(BUILD_DIR)

LNFS_LIB   := $(BUILDD)/lib$(LNFS_NAME).a
LNFS_MOUNT := $(BUILDD)/$(LNFS_NAME)-mount.bin

CXX_EXTRA_FLAGS ?= -fdiagnostics-color=auto

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
	@rm -vrf ./build/*.* ./build/debug ./build/include

.PHONY: build
build: $(LNFS_MOUNT)

.PHONY: debug
debug:
	@mkdir -vp $(BUILDD)/debug
	@$(MAKE) build BUILD_DIR=$(BUILD_DIR)/debug CXX_EXTRA_FLAGS='-ggdb -DLNFS_DEBUG'

# lnfs_build.h

$(BUILDD)/include/lnfs_build.h: Makefile $(SRCD)/include/lnfs.h
	@mkdir -vp $(BUILDD)/include
	@echo "#define LNFS_NAME \"$(LNFS_NAME)\"" >$(BUILDD)/include/lnfs_build.h

# lnfs.o

$(BUILDD)/lnfs.o: $(BUILDD)/include/lnfs_build.h $(SRCD)/include/lnfs.h $(SRCD)/lib/lnfs.cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs.o -c $(SRCD)/lib/lnfs.cpp

# lnfs_log.o

$(BUILDD)/lnfs_log.o: $(SRCD)/include/lnfs_log.h $(SRCD)/lib/lnfs_log.cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_log.o -c $(SRCD)/lib/lnfs_log.cpp

# liblnfs.a

LNFS_LIB_OBJS := $(BUILDD)/lnfs.o
LNFS_LIB_OBJS += $(BUILDD)/lnfs_log.o

$(LNFS_LIB): $(LNFS_LIB_OBJS)
	@rm -vf $(LNFS_LIB)
	@$(AR) rv $(LNFS_LIB) $(LNFS_LIB_OBJS)

.PHONY: liblinnetfs.a
liblinnetfs.a: $(LNFS_LIB)

# lnfs-mount.bin

$(LNFS_MOUNT): $(LNFS_LIB) $(SRCD)/bin/lnfs-mount/mount-main.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_MOUNT) $(SRCD)/bin/lnfs-mount/mount-main.cpp $(LNFS_LIB) $(CXX_LIBS)
