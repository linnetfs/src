.PHONY: default
default:
	@$(MAKE) build

.PHONY: docker
docker:
	@./docker/build.sh

.PHONY: build
build:
	@echo FIXME!!!
