# Compile driver of the LLM loop: links one candidate kernel against the CHEHAB
# library already built in <repo>/build and generates its SEAL CKKS code.
# A standalone project, so the repository's root CMakeLists.txt is untouched.
#
#   cmake -S llm_loop/driver -B build/llm_loop/driver -DKERNEL_DIR=<dir with kernel.inc>
cmake_minimum_required(VERSION 3.13)

# Same toolchain choice as the root CMakeLists.txt.
if(NOT DEFINED CMAKE_C_COMPILER AND EXISTS "/usr/bin/gcc")
    set(CMAKE_C_COMPILER "/usr/bin/gcc")
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER AND EXISTS "/usr/bin/g++")
    set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

project(LlmLoopDriver LANGUAGES CXX)

set(CHEHAB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "CHEHAB repository root")
set(FHECO_LIBRARY "${CHEHAB_ROOT}/build/libfheco.a" CACHE FILEPATH "CHEHAB library built by the root project")
set(KERNEL_DIR "" CACHE PATH "Directory holding kernel.inc")

if(NOT EXISTS "${FHECO_LIBRARY}")
    message(FATAL_ERROR "${FHECO_LIBRARY} not found: build the repository first (cmake --build build)")
endif()
if(NOT EXISTS "${KERNEL_DIR}/kernel.inc")
    message(FATAL_ERROR "KERNEL_DIR must contain kernel.inc")
endif()

add_executable(llm_driver driver.cpp)
target_include_directories(llm_driver PRIVATE "${CHEHAB_ROOT}/src" "${KERNEL_DIR}")
target_link_libraries(llm_driver PRIVATE "${FHECO_LIBRARY}")
target_compile_features(llm_driver PRIVATE cxx_std_17)
set_target_properties(llm_driver PROPERTIES CXX_EXTENSIONS OFF)
