
TARGET_BUILD_PATH := build-target
CLANG_BUILD_PATH := build-clang

BIN_PATH := /exports/var-som-plus/usr/bin


all: build

${TARGET_BUILD_PATH}/ninja.build:
	cmake -S . -B ${TARGET_BUILD_PATH} -G Ninja

${CLANG_BUILD_PATH}/ninja.build: export CC=/usr/bin/clang
${CLANG_BUILD_PATH}/ninja.build: export CXX=/usr/bin/clang++
${CLANG_BUILD_PATH}/ninja.build:
	cmake -S . -B ${CLANG_BUILD_PATH} -G Ninja -DCMAKE_BUILD_TYPE=Debug

compile_commands.json:
	ln -s ${CLANG_BUILD_PATH}/compile_commands.json

build: ${TARGET_BUILD_PATH}/ninja.build
	cmake --build ${TARGET_BUILD_PATH}
	cp $(TARGET_BUILD_PATH)/src/pdf-viewer $(BIN_PATH)

clang: ${CLANG_BUILD_PATH}/ninja.build compile_commands.json
	cmake --build ${CLANG_BUILD_PATH}

ci: ${TARGET_BUILD_PATH}/ninja.build
	cmake --build ${TARGET_BUILD_PATH}
	
tidy: ${CLANG_BUILD_PATH}/ninja.build compile_commands.json
	python run-clang-tidy.py -config-file=.clang-tidy -header-filter=.*  "$(CURDIR)/src/*"

targetbins:
	sudo touch $(BIN_PATH)/pdf-viewer	
	sudo chmod 777 $(BIN_PATH)/pdf-viewer

.PHONY: all build clang targetbins
