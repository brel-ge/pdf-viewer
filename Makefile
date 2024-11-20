
TARGET_BUILD_PATH := build-target
HOST_BUILD_PATH := build-host
BIN_PATH := /exports/var-som-plus/usr/bin

all: build

${TARGET_BUILD_PATH}/ninja.build:
	cmake -S . -B ${TARGET_BUILD_PATH} -G Ninja

${HOST_BUILD_PATH}/ninja.build:
	cmake -S . -B ${HOST_BUILD_PATH} -G Ninja -DCMAKE_BUILD_TYPE=Debug


build: ${TARGET_BUILD_PATH}/ninja.build
	cmake --build ${TARGET_BUILD_PATH}
	cp $(TARGET_BUILD_PATH)/src/pdf-viewer $(BIN_PATH)

host: ${HOST_BUILD_PATH}/ninja.build
	cmake --build ${HOST_BUILD_PATH}

targetbins:
	sudo touch $(BIN_PATH)/pdf-viewer	
	sudo chmod 777 $(BIN_PATH)/pdf-viewer

.PHONY: all build host targetbins
