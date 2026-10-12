# LLM optimization loop: agent_kernel compiles one candidate CHEHAB DSL kernel.
# The loop (agent/loop.py) writes the candidate to
# ${CMAKE_BINARY_DIR}/agent/kernel/kernel.inc and rebuilds this target.
set(AGENT_KERNEL_DIR ${CMAKE_BINARY_DIR}/agent/kernel)
file(MAKE_DIRECTORY ${AGENT_KERNEL_DIR})
if(NOT EXISTS ${AGENT_KERNEL_DIR}/kernel.inc)
    file(WRITE ${AGENT_KERNEL_DIR}/kernel.inc "void fhe()\n{\n  Ciphertext x(\"x\");\n  (x * x).set_output(\"y\");\n}\n")
endif()

# Not part of the default build: a failed candidate must not break other builds.
add_executable(agent_kernel EXCLUDE_FROM_ALL driver.cpp)
target_include_directories(agent_kernel PRIVATE ${AGENT_KERNEL_DIR})
target_link_libraries(agent_kernel PRIVATE fheco)
