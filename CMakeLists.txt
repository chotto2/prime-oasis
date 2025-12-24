cmake_minimum_required(VERSION 3.16)
project(prime-oasys C)

set(CMAKE_C_STANDARD 11)

add_executable(oasis_sample src/oasis_sample.c)
target_link_libraries(oasis_sample gmp m)

