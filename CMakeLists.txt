cmake_minimum_required(VERSION 3.16)
project(prime-oasis C)

set(CMAKE_C_STANDARD 11)

add_executable(oasis_layer1 src/oasis_layer1.c)
add_executable(oasis_layer2 src/oasis_layer2.c)
target_link_libraries(oasis_layer1 gmp m)
target_link_libraries(oasis_layer2 gmp m)

