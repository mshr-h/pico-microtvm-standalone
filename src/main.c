#include "pico/stdlib.h"
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <tvm/runtime/c_runtime_api.h>

#include "bundle.h"

extern const char graph_c_json[];
extern unsigned int graph_c_json_len;

extern const char params_c_bin[];
extern unsigned int params_c_bin_len;

int main(void)
{
    stdio_init_all();

    // Setup microtvm
    char *json_data = (char *)(graph_c_json);
    char *params_data = (char *)(params_c_bin);
    uint64_t params_size = params_c_bin_len;

    // create tvm_runtime
    void *handle = tvm_runtime_create(json_data, params_data, params_size, NULL);

    // prepare input tensor
    float input1_storage[1];
    float input2_storage[1];

    input1_storage[0] = 5.0;
    input2_storage[0] = 7.0;

    DLDevice dev = {kDLCPU, 0};
    DLDataType dtype = {kDLFloat, 32, 1};

    DLTensor input1;
    input1.data = input1_storage;
    input1.device = dev;
    input1.ndim = 1;
    input1.dtype = dtype;
    int64_t shape1[1] = {1};
    input1.shape = shape1;
    input1.strides = NULL;
    input1.byte_offset = 0;

    DLTensor input2;
    input2.data = input2_storage;
    input2.device = dev;
    input2.ndim = 1;
    input2.dtype = dtype;
    int64_t shape2[1] = {1};
    input2.shape = shape2;
    input2.strides = NULL;
    input2.byte_offset = 0;

    // set input tensor
    tvm_runtime_set_input(handle, "Input1", &input1);
    tvm_runtime_set_input(handle, "Input2", &input2);

    // prepare output tensor
    float output_storage[1];
    DLTensor output;
    output.data = output_storage;
    DLDevice out_dev = {kDLCPU, 0};
    output.device = out_dev;
    output.ndim = 1;
    DLDataType out_dtype = {kDLFloat, 32, 1};
    output.dtype = out_dtype;
    int64_t out_shape[1] = {1};
    output.shape = out_shape;
    output.strides = NULL;
    output.byte_offset = 0;

    for (;;)
    {
        // set input
        input1_storage[0] = 5.0;
        input2_storage[0] = 7.0;

        // run inference
        tvm_runtime_run(handle);

        // obtain output
        tvm_runtime_get_output(handle, 0, &output);

        printf("std ! %.2f\n", input1_storage[0] + input2_storage[0]);
        printf("microtvm! %.2f\n", output_storage[0]);

        sleep_ms(500);
    }

    tvm_runtime_destroy(handle);

    return 0;
}