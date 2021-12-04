#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <tvm/runtime/c_runtime_api.h>

#include "bundle.h"

extern const char build_graph_c_json[];
extern unsigned int build_graph_c_json_len;

extern const char build_params_c_bin[];
extern unsigned int build_params_c_bin_len;

int main(int argc, char** argv) {
  char* json_data = (char*)(build_graph_c_json);
  char* params_data = (char*)(build_params_c_bin);
  uint64_t params_size = build_params_c_bin_len;

  struct timeval t0, t1, t2, t3, t4, t5;
  gettimeofday(&t0, 0);

  // create tvm_runtime
  void* handle = tvm_runtime_create(json_data, params_data, params_size, argv[0]);
  gettimeofday(&t1, 0);

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

  // set input
  tvm_runtime_set_input(handle, "Input1", &input1);
  tvm_runtime_set_input(handle, "Input2", &input2);
  gettimeofday(&t2, 0);

  // run inference
  tvm_runtime_run(handle);
  gettimeofday(&t3, 0);

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

  // obtain output
  tvm_runtime_get_output(handle, 0, &output);
  gettimeofday(&t4, 0);

  // print result
  printf("input : %f, %f\n", input1_storage[0], input2_storage[0]);
  printf("output: %f\n", output_storage[0]);
  
  tvm_runtime_destroy(handle);
  gettimeofday(&t5, 0);

  printf(
      "timing: %.2f ms (create), %.2f ms (set_input), %.2f ms (run), "
      "%.2f ms (get_output), %.2f ms (destroy)\n",
      (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) / 1000.f,
      (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000.f,
      (t3.tv_sec - t2.tv_sec) * 1000 + (t3.tv_usec - t2.tv_usec) / 1000.f,
      (t4.tv_sec - t3.tv_sec) * 1000 + (t4.tv_usec - t3.tv_usec) / 1000.f,
      (t5.tv_sec - t4.tv_sec) * 1000 + (t5.tv_usec - t4.tv_usec) / 1000.f);

  return 0;
}
