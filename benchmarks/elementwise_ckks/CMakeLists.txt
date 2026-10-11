file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/elementwise_ckks/he)
add_executable(elementwise_ckks elementwise_ckks.cpp)
target_link_libraries(elementwise_ckks PRIVATE fheco)

# Files the generated CKKS program needs next to it (same layout as the BFV
# benchmarks, with benchmarks/ckks_utils.* instead of benchmarks/utils.*).
set(ELEMENTWISE_CKKS_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/elementwise_ckks/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/elementwise_ckks/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/elementwise_ckks/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/elementwise_ckks/he/CMakeLists.txt"
    "${CMAKE_SOURCE_DIR}/benchmarks/elementwise_ckks/generate_elementwise_ckks.py|${CMAKE_BINARY_DIR}/benchmarks/elementwise_ckks/generate_elementwise_ckks.py"
)

set(ELEMENTWISE_CKKS_COPY_INDEX 0)
foreach(pair ${ELEMENTWISE_CKKS_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_elementwise_ckks_${ELEMENTWISE_CKKS_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(elementwise_ckks copy_elementwise_ckks_${ELEMENTWISE_CKKS_COPY_INDEX})
    math(EXPR ELEMENTWISE_CKKS_COPY_INDEX "${ELEMENTWISE_CKKS_COPY_INDEX} + 1")
endforeach()
