file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/batchnorm_ckks/he)
add_executable(batchnorm_ckks batchnorm_ckks.cpp)
target_link_libraries(batchnorm_ckks PRIVATE fheco)

# Input generator: reuses the BatchNorm folding and packing helpers of the
# direct-SEAL reference module (they live in a SEAL-dependent translation unit).
find_package(SEAL 4.1 REQUIRED)
add_executable(
    generate_batchnorm_ckks
    generate_batchnorm_ckks.cpp
    ${CMAKE_SOURCE_DIR}/benchmarks/dl_operators_ckks/fhe_operators.cpp
)
target_include_directories(generate_batchnorm_ckks PRIVATE ${CMAKE_SOURCE_DIR}/benchmarks/dl_operators_ckks)
target_compile_features(generate_batchnorm_ckks PRIVATE cxx_std_17)
target_link_libraries(generate_batchnorm_ckks PRIVATE SEAL::seal)
set_target_properties(
    generate_batchnorm_ckks PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/batchnorm_ckks)

# Files the generated CKKS program needs next to it.
set(BATCHNORM_CKKS_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/batchnorm_ckks/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/batchnorm_ckks/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/batchnorm_ckks/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/batchnorm_ckks/he/CMakeLists.txt"
)

set(BATCHNORM_CKKS_COPY_INDEX 0)
foreach(pair ${BATCHNORM_CKKS_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_batchnorm_ckks_${BATCHNORM_CKKS_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(batchnorm_ckks copy_batchnorm_ckks_${BATCHNORM_CKKS_COPY_INDEX})
    math(EXPR BATCHNORM_CKKS_COPY_INDEX "${BATCHNORM_CKKS_COPY_INDEX} + 1")
endforeach()
