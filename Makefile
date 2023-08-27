# Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com>
# See LICENSE file.

LNFS_NAME ?= linnetfs
LNFS_MODE ?= passthrough

BUILD_DIR ?= build

SRCD := $(PWD)
BUILDD := $(SRCD)/$(BUILD_DIR)

LNFS_LIB := $(BUILDD)/lib$(LNFS_NAME).a
LNFS_LIB_DEPS := $(LNFS_LIB)
LNFS_LIB_DEPS += $(SRCD)/include/lnfs_log.hpp $(SRCD)/lib/lnfs_log.cpp

CXX_EXTRA_FLAGS ?= -fdiagnostics-color=auto

CXX_FLAGS := -std=c++23 -Wall -pedantic -O2 -fPIC -pie
CXX_FLAGS += -I/usr/include/fuse3 -I$(SRCD)/include
CXX_FLAGS += -I$(BUILDD)/include
CXX_FLAGS += -DLNFS_MODE_$(LNFS_MODE)
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

LNFS_MOUNT := $(BUILDD)/$(LNFS_NAME)-mount.bin

.PHONY: build
build: $(LNFS_MOUNT)

.PHONY: debug
debug:
	@mkdir -vp $(BUILDD)/debug
	@$(MAKE) build BUILD_DIR=$(BUILD_DIR)/debug CXX_EXTRA_FLAGS='-ggdb -DLNFS_DEBUG'

# lnfs_build.h

$(BUILDD)/include/lnfs_build.h: Makefile $(SRCD)/include/lnfs.hpp
	@mkdir -vp $(BUILDD)/include
	@echo "#define LNFS_NAME \"$(LNFS_NAME)\"" >$(BUILDD)/include/lnfs_build.h

# lnfs_utils.o

$(BUILDD)/lnfs_utils.o: $(BUILDD)/include/lnfs_build.h $(SRCD)/include/lnfs_utils.hpp $(SRCD)/lib/lnfs_utils.cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_utils.o -c $(SRCD)/lib/lnfs_utils.cpp

# lnfs_$(LNFS_MODE).o

$(BUILDD)/lnfs_$(LNFS_MODE).o: $(SRCD)/include/lnfs_$(LNFS_MODE).hpp $(SRCD)/lib/lnfs_$(LNFS_MODE).cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_$(LNFS_MODE).o -c $(SRCD)/lib/lnfs_$(LNFS_MODE).cpp

# liblnfs.a

LNFS_LIB_OBJS := $(BUILDD)/lnfs_utils.o
LNFS_LIB_OBJS += $(BUILDD)/lnfs_$(LNFS_MODE).o

$(LNFS_LIB): $(LNFS_LIB_OBJS)
	@rm -vf $(LNFS_LIB)
	@$(AR) rv $(LNFS_LIB) $(LNFS_LIB_OBJS)

.PHONY: liblinnetfs.a
liblinnetfs.a: $(LNFS_LIB)

# lnfs-mount.bin

$(LNFS_MOUNT): $(LNFS_LIB_DEPS) $(SRCD)/bin/lnfs-mount/mount-main.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_MOUNT) $(SRCD)/bin/lnfs-mount/mount-main.cpp $(LNFS_LIB) $(CXX_LIBS)
