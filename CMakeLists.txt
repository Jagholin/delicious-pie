cmake_minimum_required(VERSION 2.8.11)
set (CMAKE_PREFIX_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64")
project(delicious-pie)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Boost 1.54.0 REQUIRED)

add_subdirectory("src")
