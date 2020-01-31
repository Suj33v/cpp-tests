cmake_minimum_required(VERSION 3.12)
project("cpp-tests")
set(CMAKE_CXX_STANDARD 14)
enable_testing()

# generate compile_commands.json for use by Intellisense.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Boost header files
set(BOOST_ROOT "$ENV{HOME}/bin/boost/binaries")
find_package(Boost 1.72)
message("include paths ${Boost_INCLUDE_DIRS}")
include_directories(${Boost_INCLUDE_DIRS})

# Project source files
add_subdirectory(src)
