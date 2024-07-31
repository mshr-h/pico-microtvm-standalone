#!/bin/bash
source_dir="tvm"
build_options="-DUSE_RELAY_DEBUG=ON -DUSE_PROFILER=ON -DUSE_LLVM=ON -DUSE_MICRO_STANDALONE_RUNTIME=ON -DUSE_MICRO=ON"
clean_build_dir=false

while test $# -gt 0; do
  case "$1" in
    -h|--help)
      echo "options:"
      echo "-h, --help  show brief help"
      echo "--clean     cleanup build directory"
      exit 0
      ;;
    --clean)
      shift
      clean_build_dir=true
      ;;
    *)
      break
      ;;
  esac
done

# clone repo or pull
if [ -d "$source_dir" ]; then
  cd $source_dir
  git pull
  cd ..
else
  git clone --recursive --branch main https://github.com/apache/tvm $source_dir
fi

# build
cd $source_dir
if [ $clean_build_dir = true ]; then
  echo "cleanning build directory..."
  rm -rf build
fi
git submodule sync && git submodule update --init --recursive
cmake -S . -B build -G Ninja $build_options
cmake --build build

# install python package
uv pip install -e python --config-setting editable-mode=compat
