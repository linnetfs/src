LNFS_NAME ?= lnfs

SRCD := $(PWD)
BUILDD := $(SRCD)/build

CXX_FLAGS := -std=c++23 -Wall -pedantic -O2 -fPIC -I/usr/include/fuse3 -I$(SRCD)/include
CXX_LIBS := $(BUILDD)/lib$(LNFS_NAME).o -lfuse3 -lpthread

.PHONY: default
default: build

.PHONY: build
build: $(BUILDD)/$(LNFS_NAME).bin

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: clean
clean:
	@rm -vf ./build/*.o ./build/*.bin

$(BUILDD)/lib$(LNFS_NAME).o: include/*.h lib/*.cpp
	@install -d -m 0750 ./build
	$(CXX) $(CXX_FLAGS) -c -o $(BUILDD)/lib$(LNFS_NAME).o $(SRCD)/lib/lnfs.cpp

$(BUILDD)/$(LNFS_NAME).bin: $(BUILDD)/lib$(LNFS_NAME).o $(SRCD)/bin/lnfs/main.cpp
	@install -d -m 0750 ./build
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/$(LNFS_NAME).bin $(SRCD)/bin/lnfs/main.cpp $(CXX_LIBS)
