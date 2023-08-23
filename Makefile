LNFS_NAME ?= lnfs
CXX_FLAGS := -std=c++23 -Wall -pedantic -O2 -fPIC -I/usr/include/fuse3 -lfuse3 -lpthread -I$(PWD)/lib

.PHONY: default
default: build

.PHONY: build
build: $(LNFS_NAME).bin

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: clean
clean:
	@rm -vf *.o *.bin

lib$(LNFS_NAME).o: lib/lnfs.cpp
	$(CXX) $(CXX_FLAGS) -c -o lib$(LNFS_NAME).o lib/lnfs.cpp

$(LNFS_NAME).bin: lib$(LNFS_NAME).o bin/lnfs/main.cpp
	$(CXX) $(CXX_FLAGS) -o $(LNFS_NAME).bin lib$(LNFS_NAME).o bin/lnfs/main.cpp
