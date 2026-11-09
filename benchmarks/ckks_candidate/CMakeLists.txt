file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/ckks_candidate/he)
add_executable(ckks_candidate ckks_candidate.cpp)
target_link_libraries(ckks_candidate PRIVATE fheco)

# Files the generated CKKS program needs next to it (same layout as the BFV
# benchmarks, with benchmarks/ckks_utils.* instead of benchmarks/utils.*).
set(CKKS_CANDIDATE_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/ckks_candidate/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/ckks_candidate/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_candidate/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/ckks_candidate/he/CMakeLists.txt"
)

set(CKKS_CANDIDATE_COPY_INDEX 0)
foreach(pair ${CKKS_CANDIDATE_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_ckks_candidate_${CKKS_CANDIDATE_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(ckks_candidate copy_ckks_candidate_${CKKS_CANDIDATE_COPY_INDEX})
    math(EXPR CKKS_CANDIDATE_COPY_INDEX "${CKKS_CANDIDATE_COPY_INDEX} + 1")
endforeach()
