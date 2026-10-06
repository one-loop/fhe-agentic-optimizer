file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/extract_embedding_ckks/he)
add_executable(extract_embedding_ckks extract_embedding_ckks.cpp)
target_link_libraries(extract_embedding_ckks PRIVATE fheco)

set(EXTRACT_EMBEDDING_CKKS_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/extract_embedding_ckks/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/extract_embedding_ckks/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/extract_embedding_ckks/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/extract_embedding_ckks/he/CMakeLists.txt"
    "${CMAKE_SOURCE_DIR}/benchmarks/extract_embedding_ckks/generate_extract_embedding_ckks.py|${CMAKE_BINARY_DIR}/benchmarks/extract_embedding_ckks/generate_extract_embedding_ckks.py"
)

set(EXTRACT_EMBEDDING_CKKS_COPY_INDEX 0)
foreach(pair ${EXTRACT_EMBEDDING_CKKS_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_extract_embedding_ckks_${EXTRACT_EMBEDDING_CKKS_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(extract_embedding_ckks copy_extract_embedding_ckks_${EXTRACT_EMBEDDING_CKKS_COPY_INDEX})
    math(EXPR EXTRACT_EMBEDDING_CKKS_COPY_INDEX "${EXTRACT_EMBEDDING_CKKS_COPY_INDEX} + 1")
endforeach()
