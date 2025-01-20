# Set variables
target-build-path := "build-target"
clang-build-path := "build-clang"
bin-path := "/exports/var-som-plus/usr/bin"
YOCTO_ENV := ". /opt/fslc-xwayland/pilatus-sdk-4.1/environment-setup-armv8a-fslc-linux &&"
UID := `id -u`
GID := `id -g`
DOCKER_RUN := "docker run --rm -u " + UID + ":" + GID + " -v " + `pwd` + ":/workspace -w /workspace pilatus-build"

# Default recipe
default: build

# Create build directory and generate ninja build files for target
create-target-build:
    {{ YOCTO_ENV }} cmake -S . -B {{target-build-path}} -G Ninja

# Create build directory and generate ninja build files for clang
create-clang-build:
    #!/usr/bin/env bash
    export CC=clang
    export CXX=clang++
    cmake -S . -B {{clang-build-path}} -G Ninja -DCMAKE_BUILD_TYPE=Debug

# Create symlink for compile commands
create-compile-commands:
    ln -s {{clang-build-path}}/compile_commands.json

# Build the project and install binary
docker-build: create-target-build
    {{ YOCTO_ENV }} cmake --build {{target-build-path}}

build:
    {{ DOCKER_RUN }} just docker-build
    cp {{target-build-path}}/src/pdf-viewer {{bin-path}}

# Build with clang
clang: create-clang-build create-compile-commands
    cmake --build {{clang-build-path}}

# CI build
ci: create-target-build
    cmake --build {{target-build-path}}

# Run clang-tidy checks
tidy: create-clang-build create-compile-commands
    python run-clang-tidy.py -config-file=.clang-tidy -header-filter=.* "$(pwd)/src/*"

# Setup target binaries
targetbins:
    sudo touch {{bin-path}}/pdf-viewer
    sudo chmod 777 {{bin-path}}/pdf-viewer
