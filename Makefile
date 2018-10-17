DIST_DIR ?= ${PWD}/dist
BUILD_DIR ?= ${PWD}/build

.PHONY: build
build:
	@if [[ -d ${BUILD_DIR} ]]; then rm -rf ${BUILD_DIR}; fi
	@mkdir ${BUILD_DIR}
	@cd ${BUILD_DIR}; cmake ..; make; ctest

.PHONY: install
install: build
	@cd ${BUILD_DIR}; make install

.PHONY: build
dist: build
	@cd ${BUILD_DIR}; cpack --config CPackConfig.cmake -B ${DIST_DIR}

.PHONY: build_src
dist_src: build
	@cd build; cpack --config CPackSourceConfig.cmake -B ${DIST_DIR}
	
.PHONY: build_bin
dist_bin: dist
