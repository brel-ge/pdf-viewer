cmake_minimum_required(VERSION 3.16)

project(pdfviewer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(QT_QML_GENERTATE_QMLLS_INI ON)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)


# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER REQUIRED poppler-qt6)

# Configure Qt
qt_standard_project_setup()

if(ENABLE_COVERAGE)
  # set compiler flags
  set(CMAKE_CXX_FLAGS "-O0 --coverage")
endif()


add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
