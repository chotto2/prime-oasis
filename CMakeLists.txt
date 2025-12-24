cmake_minimum_required(VERSION 3.16)
project(prime-oasis C)

set(CMAKE_C_STANDARD 11)

add_executable(oasis_layer1 src/oasis_layer1.c)
target_link_libraries(oasis_layer1 gmp m)

