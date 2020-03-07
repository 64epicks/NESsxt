cmake_minimum_required(VERSION 2.8)
project(NESsxt)

find_package(SDL2 REQUIRED)

file (GLOB src
    "src/*.cpp"
)

add_definitions(-DNO_NAMESPACE)
include_directories(include/)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(NESsxt ${src})
target_link_libraries(NESsxt SDL2)