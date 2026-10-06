cmake_minimum_required(VERSION 3.13)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(CHEHABCKKSOperators LANGUAGES CXX)
endif()

find_package(SEAL 4.1 REQUIRED)

add_executable(
    ckks_operator_demo
    fhe_operators.cpp
    main.cpp
)

target_compile_features(ckks_operator_demo PRIVATE cxx_std_17)
set_target_properties(ckks_operator_demo PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(ckks_operator_demo PRIVATE SEAL::seal)
