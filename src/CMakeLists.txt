cmake_minimum_required(VERSION 3.0)
project(Tron)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# include dirs
set(INCLUDE_DIRS /opt/tron/src)

# install prefix
set(CMAKE_INSTALL_PREFIX /opt/tron/)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# add_subdirectory(core)
# add_subdirectory(common)
add_subdirectory(test)
