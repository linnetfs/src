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
	@rm -vf ./build/*.*

$(BUILDD)/lib$(LNFS_NAME).o: $(SRCD)/include/*.h $(SRCD)/lib/*.cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/lib$(LNFS_NAME).o -c $(SRCD)/lib/lnfs.cpp

$(BUILDD)/$(LNFS_NAME).bin: $(BUILDD)/lib$(LNFS_NAME).o $(SRCD)/bin/lnfs/main.cpp
	$(CXX) $(CXX_FLAGS) -o $(BUILDD)/$(LNFS_NAME).bin $(SRCD)/bin/lnfs/main.cpp $(CXX_LIBS)
