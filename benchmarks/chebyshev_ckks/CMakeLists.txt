file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/chebyshev_ckks/he)

# Coefficient sets and reference activations come from the direct-SEAL
# reference module (header-only parts).
set(CHEBYSHEV_CKKS_REFERENCE_DIR ${CMAKE_SOURCE_DIR}/benchmarks/dl_operators_ckks)

add_executable(chebyshev_ckks chebyshev_ckks.cpp)
target_include_directories(chebyshev_ckks PRIVATE ${CHEBYSHEV_CKKS_REFERENCE_DIR})
target_link_libraries(chebyshev_ckks PRIVATE fheco)

add_executable(generate_chebyshev_ckks generate_chebyshev_ckks.cpp)
target_include_directories(generate_chebyshev_ckks PRIVATE ${CHEBYSHEV_CKKS_REFERENCE_DIR})
target_compile_features(generate_chebyshev_ckks PRIVATE cxx_std_17)

# Files the generated CKKS program needs next to it.
set(CHEBYSHEV_CKKS_COPIES
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.cpp|${CMAKE_BINARY_DIR}/benchmarks/chebyshev_ckks/he/ckks_utils.cpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/ckks_utils.hpp|${CMAKE_BINARY_DIR}/benchmarks/chebyshev_ckks/he/ckks_utils.hpp"
    "${CMAKE_SOURCE_DIR}/benchmarks/chebyshev_ckks/he/CMakeLists.txt|${CMAKE_BINARY_DIR}/benchmarks/chebyshev_ckks/he/CMakeLists.txt"
)

set(CHEBYSHEV_CKKS_COPY_INDEX 0)
foreach(pair ${CHEBYSHEV_CKKS_COPIES})
    string(REPLACE "|" ";" pair_list "${pair}")
    list(GET pair_list 0 source_file)
    list(GET pair_list 1 destination_file)
    add_custom_target(copy_chebyshev_ckks_${CHEBYSHEV_CKKS_COPY_INDEX} ALL
        COMMAND ${CMAKE_COMMAND} -E copy ${source_file} ${destination_file}
        COMMENT "Copying ${source_file} to ${destination_file}"
    )
    add_dependencies(chebyshev_ckks copy_chebyshev_ckks_${CHEBYSHEV_CKKS_COPY_INDEX})
    math(EXPR CHEBYSHEV_CKKS_COPY_INDEX "${CHEBYSHEV_CKKS_COPY_INDEX} + 1")
endforeach()
