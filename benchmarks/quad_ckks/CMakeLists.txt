file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/quad_ckks/he)
add_executable(quad_ckks quad_ckks.cpp)
target_link_libraries(quad_ckks PRIVATE fheco)

# Files the generated CKKS program needs next to it (same layout as the BFV
# benchmarks, with benchmarks/ckks_utils.* instead of benchmarks/utils.*).
set(QUAD_CKKS_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/quad_ckks/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/quad_ckks/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/quad_ckks/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/quad_ckks/he/CMakeLists.txt"
    "${CMAKE_SOURCE_DIR}/benchmarks/quad_ckks/generate_quad_ckks.py|${CMAKE_BINARY_DIR}/benchmarks/quad_ckks/generate_quad_ckks.py"
)

set(QUAD_CKKS_COPY_INDEX 0)
foreach(pair ${QUAD_CKKS_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_quad_ckks_${QUAD_CKKS_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(quad_ckks copy_quad_ckks_${QUAD_CKKS_COPY_INDEX})
    math(EXPR QUAD_CKKS_COPY_INDEX "${QUAD_CKKS_COPY_INDEX} + 1")
endforeach()
