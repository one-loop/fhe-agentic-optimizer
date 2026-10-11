file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/relu_ckks/he)
add_executable(relu_ckks relu_ckks.cpp)
target_link_libraries(relu_ckks PRIVATE fheco)

# Files the generated CKKS program needs next to it (same layout as the BFV
# benchmarks, with benchmarks/ckks_utils.* instead of benchmarks/utils.*).
set(RELU_CKKS_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/relu_ckks/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/relu_ckks/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/relu_ckks/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/relu_ckks/he/CMakeLists.txt"
    "${CMAKE_SOURCE_DIR}/benchmarks/relu_ckks/generate_relu_ckks.py|${CMAKE_BINARY_DIR}/benchmarks/relu_ckks/generate_relu_ckks.py"
    "${CMAKE_SOURCE_DIR}/benchmarks/relu_ckks/relu_sign_coeffs.json|${CMAKE_BINARY_DIR}/benchmarks/relu_ckks/relu_sign_coeffs.json"
)

set(RELU_CKKS_COPY_INDEX 0)
foreach(pair ${RELU_CKKS_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_relu_ckks_${RELU_CKKS_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(relu_ckks copy_relu_ckks_${RELU_CKKS_COPY_INDEX})
    math(EXPR RELU_CKKS_COPY_INDEX "${RELU_CKKS_COPY_INDEX} + 1")
endforeach()
