cmake_minimum_required(VERSION 3.10)
project(Tmid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Build the main application
add_executable(tmid ${SOURCES})

# Build the installer tool
add_executable(tmid-install tools/tmid-install.cpp)