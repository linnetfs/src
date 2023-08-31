# Copyright (c) Jeremías Casteglione <jrmsdev@gmail.com>
# See LICENSE file.

LNFS_NAME ?= linnetfs
LNFS_MODE ?= passthrough

BUILD_DIR ?= build/$(LNFS_MODE)

SRCD := $(PWD)
BUILDD := $(SRCD)/$(BUILD_DIR)

LNFS_LIB := $(BUILDD)/lib$(LNFS_NAME).a

LNFS_LIB_OBJS := $(BUILDD)/lnfs_$(LNFS_MODE).o
LNFS_LIB_OBJS += $(BUILDD)/lnfs_file.o
LNFS_LIB_OBJS += $(BUILDD)/lnfs_memfs.o

LNFS_BUILD_H := $(BUILDD)/include/lnfs_build.hpp

LNFS_BUILD_H_DEPS := Makefile
LNFS_BUILD_H_DEPS := $(SRCD)/include/lnfs.hpp

LNFS_SRC_DEPS := $(LNFS_BUILD_H)
LNFS_SRC_DEPS += $(SRCD)/include/lnfs.hpp
LNFS_SRC_DEPS += $(SRCD)/include/lnfs_fuse.hpp
LNFS_SRC_DEPS += $(SRCD)/include/lnfs_log.hpp

LNFS_BIN_DEPS := $(LNFS_LIB) $(LNFS_SRC_DEPS)

LNFS_MOUNT := $(BUILDD)/$(LNFS_NAME)-mount.bin

CXX_EXTRA_FLAGS ?= -O2

CXX_FLAGS := -std=c++23 -Wall -pedantic -fPIC -pie
CXX_FLAGS += -I/usr/include/fuse3 -I$(SRCD)/include
CXX_FLAGS += -I$(BUILDD)/include
CXX_FLAGS += $(CXX_EXTRA_FLAGS)
CXX_FLAGS += -fdiagnostics-color=auto
CXX_FLAGS += -DLNFS_MODE_$(LNFS_MODE)

CXX_LIBS := -lfuse3 -lpthread

.PHONY: default
default: build

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: clean
clean:
	@rm -vrf ./build/*

.PHONY: build
build: $(LNFS_MOUNT)

.PHONY: debug
debug:
	@$(MAKE) build BUILD_DIR=$(BUILD_DIR)/debug CXX_EXTRA_FLAGS='-O0 -ggdb -DLNFS_DEBUG'

.PHONY: debug-all
debug-all:
	@$(MAKE) debug LNFS_MODE=passthrough
	@$(MAKE) debug LNFS_MODE=memory

.PHONY: all
all:
	@$(MAKE) build LNFS_MODE=passthrough
	@$(MAKE) build LNFS_MODE=memory

# lnfs_build.h

$(LNFS_BUILD_H): $(LNFS_BUILD_H_DEPS)
	@mkdir -vp $(BUILDD)/include
	@echo '#pragma once'                        >$(LNFS_BUILD_H)
	@echo "#define LNFS_NAME \"$(LNFS_NAME)\"" >>$(LNFS_BUILD_H)
	@echo "#define LNFS_MODE \"$(LNFS_MODE)\"" >>$(LNFS_BUILD_H)

# lnfs_file.o

LNFS_FILE_DEPS := $(LNFS_SRC_DEPS)
LNFS_FILE_DEPS += $(SRCD)/include/lnfs_file.hpp
LNFS_FILE_DEPS += $(SRCD)/lib/lnfs_file.cpp

$(BUILDD)/lnfs_file.o: $(LNFS_FILE_DEPS)
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_file.o -c $(SRCD)/lib/lnfs_file.cpp

# lnfs_memfs.o

LNFS_MEMFS_DEPS := $(LNFS_SRC_DEPS)
LNFS_MEMFS_DEPS += $(SRCD)/include/lnfs_memfs.hpp
LNFS_MEMFS_DEPS += $(SRCD)/lib/lnfs_memfs.cpp

$(BUILDD)/lnfs_memfs.o: $(LNFS_MEMFS_DEPS)
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_memfs.o -c $(SRCD)/lib/lnfs_memfs.cpp

# lnfs_$(LNFS_MODE).o

LNFS_MODE_DEPS := $(LNFS_SRC_DEPS)
LNFS_MODE_DEPS += $(SRCD)/include/lnfs_$(LNFS_MODE).hpp
LNFS_MODE_DEPS += $(SRCD)/lib/lnfs_$(LNFS_MODE).cpp

$(BUILDD)/lnfs_$(LNFS_MODE).o: $(LNFS_MODE_DEPS)
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lnfs_$(LNFS_MODE).o -c $(SRCD)/lib/lnfs_$(LNFS_MODE).cpp

# liblnfs.a

$(LNFS_LIB): $(LNFS_LIB_OBJS)
	@rm -vf $(LNFS_LIB)
	@$(AR) rv $(LNFS_LIB) $(LNFS_LIB_OBJS)

.PHONY: liblinnetfs.a
liblinnetfs.a: $(LNFS_LIB)

# lnfs-mount.bin

$(LNFS_MOUNT): $(LNFS_BIN_DEPS) $(SRCD)/bin/lnfs-mount/mount-main.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_MOUNT) $(SRCD)/bin/lnfs-mount/mount-main.cpp $(LNFS_LIB) $(CXX_LIBS)
