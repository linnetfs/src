LNFS_NAME ?= lnfs
CXX_FLAGS := -std=c++23 -Wall -pedantic -O2 -fPIC -I/usr/include/fuse3 -I$(PWD)/lib
CXX_LIBS := $(PWD)/build/lib$(LNFS_NAME).o -lfuse3 -lpthread

.PHONY: default
default: build

.PHONY: build
build: build/$(LNFS_NAME).bin

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: clean
clean:
	@rm -rf ./build

build/lib$(LNFS_NAME).o: lib/lnfs.cpp
	@install -d -m 0750 ./build
	$(CXX) $(CXX_FLAGS) -c -o build/lib$(LNFS_NAME).o lib/lnfs.cpp

build/$(LNFS_NAME).bin: build/lib$(LNFS_NAME).o bin/lnfs/main.cpp
	@install -d -m 0750 ./build
	$(CXX) $(CXX_FLAGS) -o build/$(LNFS_NAME).bin bin/lnfs/main.cpp $(CXX_LIBS)
