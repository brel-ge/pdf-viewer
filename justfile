# Set variables
target-build-path := "build-target"
clang-build-path := "build-clang"
ci-build-path := "build-ci"
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
    #!/usr/bin/env bash
    if [ ! -f compile_commands.json ]; then 
        ln -s {{clang-build-path}}/compile_commands.json
    fi


# Build the project and install binary
docker-build: create-target-build
    {{ YOCTO_ENV }} cmake --build {{target-build-path}}

build:
    {{ DOCKER_RUN }} just docker-build
    cp {{target-build-path}}/src/pdf-viewer {{bin-path}}

# Build with clang
clang: create-clang-build create-compile-commands
    cmake --build {{clang-build-path}}

ci-create-target-build:
    cmake -S . -B {{ci-build-path}} -G Ninja -DENABLE_COVERAGE=true -DCMAKE_BUILD_TYPE=Debug
# CI build
ci: ci-create-target-build
    cmake --build {{ci-build-path}}

test: ci-create-target-build
    #!/usr/bin/env bash
    cmake --build {{ci-build-path}}
    export QT_QPA_PLATFORM=offscreen
    {{ci-build-path}}/tests/tst-pdf-model/tst-pdf-model -junitxml -o test.xml
    lcov --directory . --capture --output-file coverage.info
    lcov --remove coverage.info '/usr/*' 'build-ci/*' 'pdf-viewer/Qt' --output-file coverage.info --ignore-errors unused

# Run clang-tidy checks
tidy: create-clang-build create-compile-commands
    python run-clang-tidy.py -config-file=.clang-tidy -header-filter=.* "{{justfile_directory()}}/src/*"

# Setup target binaries
targetbins:
    sudo touch {{bin-path}}/pdf-viewer
    sudo chmod 777 {{bin-path}}/pdf-viewer
