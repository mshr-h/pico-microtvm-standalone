mkdir -p src/tvm
cp -R 3rdparty/tvm/build/crt_config                src/tvm/
cp -R 3rdparty/tvm/build/standalone_crt            src/tvm/
cp 3rdparty/tvm/apps/bundle_deploy/bundle_static.c src/bundle_static.c
cp 3rdparty/tvm/apps/bundle_deploy/bundle.h        src/bundle.h
