cmake_minimum_required(VERSION 3.16)
project(prime-oasis C)

set(CMAKE_C_STANDARD 11)

add_executable(oasis_layer1 src/oasis_layer1.c)
add_executable(oasis_layer2 src/oasis_layer2.c)
add_executable(oasis_layer3 src/oasis_layer3.c)
add_executable(oasis_divs   src/oasis_divs.c)
add_executable(prime_oasis  src/prime_oasis.c)
target_link_libraries(oasis_layer1 gmp m)
target_link_libraries(oasis_layer2 gmp m)
target_link_libraries(oasis_layer3 gmp m)
target_link_libraries(oasis_divs   gmp m)
target_link_libraries(prime_oasis  gmp m)

