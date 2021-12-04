import argparse
import os
from tvm import relay
import tvm
from tvm import runtime
import logging

def build_model_add(opts):
    import onnx
    onnx_path = opts.model
    onnx_model = onnx.load(onnx_path)

    input1_name = "Input1"
    input2_name = "Input2"
    shape_dict = {input1_name: [1], input2_name: [1]}
    mod, params = relay.frontend.from_onnx(onnx_model, shape_dict)

    runtime_name = "c"
    file_format_str = "{name}_c.{ext}"
    with tvm.transform.PassContext(opt_level=3, config={"tir.disable_vectorize": True}):
        lib = relay.build(
            mod, f"c --runtime={runtime_name} --system-lib --link-params", params=params
        )

    build_dir = os.path.abspath(opts.out_dir)
    if not os.path.isdir(build_dir):
        os.makedirs(build_dir)

    lib_file_name = os.path.join(build_dir, file_format_str.format(name="model", ext="tar"))
    lib.export_library(lib_file_name)

    with open(
        os.path.join(build_dir, file_format_str.format(name="graph", ext="json")), "w"
    ) as f_graph_json:
        f_graph_json.write(lib.get_graph_json())
    with open(
        os.path.join(build_dir, file_format_str.format(name="params", ext="bin")), "wb"
    ) as f_params:
        f_params.write(runtime.save_param_dict(lib.get_params()))


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--out-dir", default=".")
    parser.add_argument("-m", "--model", required=True)
    opts = parser.parse_args()

    build_model_add(opts)
