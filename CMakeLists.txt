cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(pico_cpp_example C CXX ASM)

# initialize the SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# add example code
add_executable(microtvm
    lib0.c
    lib1.c
    graph_c.json.c
    params_c.bin.c
    src/main.c
    src/bundle_static.c
    src/tvm/standalone_crt/src/runtime/crt/common/crt_runtime_api.c
    src/tvm/standalone_crt/src/runtime/crt/graph_executor/graph_executor.c
    src/tvm/standalone_crt/src/runtime/crt/memory/page_allocator.c
    src/tvm/standalone_crt/src/runtime/crt/common/packed_func.c
    src/tvm/standalone_crt/src/runtime/crt/common/func_registry.c
    src/tvm/standalone_crt/src/runtime/crt/graph_executor/load_json.c
    src/tvm/standalone_crt/src/runtime/crt/common/crt_runtime_api.c
    src/tvm/standalone_crt/src/runtime/crt/common/ndarray.c
    src/tvm/standalone_crt/src/runtime/crt/common/crt_backend_api.c
)

target_include_directories(microtvm
    PUBLIC src/tvm/standalone_crt/include
)

target_include_directories(microtvm
    PUBLIC src/tvm/crt_config
)

pico_enable_stdio_usb(microtvm 1)
pico_enable_stdio_uart(microtvm 0)

# add pico_stdlib library which aggregates commonly used features 
target_link_libraries(microtvm pico_stdlib)

# create extra output files(map/bin/hex).
pico_add_extra_outputs(microtvm)
