# microTVM example project for Raspberry Pi Pico

This is a very basic microTVM example for Raspberry Pi Pico.

- TVM v0.17
- Ubuntu 24.04
- Python 3.12

## Getting Started (Example for Ubuntu 24.04)

Install CMake and cross compiler.

```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi
```

Clone the repository.

```bash
git clone https://github.com/mshr-h/pico-microtvm-standalone
cd pico-microtvm-standalone
```

Create python virtual env and install python dependencies.

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

Build and install TVM from source.

```bash
cd 3rdparty
./build-tvm.sh
cd ..
```

Build model with TVM.

```bash
python build_module.py -m add.onnx
```

Extract tar and generate hexdumps.

```
tar xvf model_c.tar
xxd -i graph_c.json > graph_c.json.c
xxd -i params_c.bin > params_c.bin.c
```

- `graph_c.json.c`
  - Hexdump for computational graph
- `params_c.bin.c`
  - Hexdump for the parameter of the graph
- `lib0.c`
  - Operator registry
- `lib1.c`
  - Compiled operators

Copy the TVM runtime files from 3rdparty/tvm

```bash
./copy-tvm-crt.sh
```

Fix some errors before building the code.

```bash
patch -p1 < diff.patch
```

Configure and build it.

If `PICO_SDK_PATH` environment variable isn't set, cmake will automatically download [pico-sdk](https://github.com/raspberrypi/pico-sdk) to the `build/_dep` directory.

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

Connect the Pico to the Host PC with pressing the BOOTSEL button.

Copy the binary to Raspberry Pi Pico.

```bash
cp microtvm.uf2 /path/to/pico_storage
```

Connect to the Pico with UART. 
The default baud rate is 115200.

You can see the inference result.

## Additional resources

- [raspberrypi/pico-examples](https://github.com/raspberrypi/pico-examples)
- [raspberrypi/pico-sdk](https://github.com/raspberrypi/pico-sdk)
- [Raspberry Pi Pico SDK: Raspberry Pi Pico SDK](https://raspberrypi.github.io/pico-sdk-doxygen/)
- [Raspberry Pi Pico C/C++ SDK](https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf)
- [mshr-h/pico-cpp-minimum-example: A minimum C++ example for Raspberry Pi Pico](https://github.com/mshr-h/pico-cpp-minimum-example)
