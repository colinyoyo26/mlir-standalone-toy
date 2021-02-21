#! /usr/bin/env bash

mkdir -p build && cd build
cmake -G Ninja .. -DMLIR_DIR=../build/lib/cmake/mlir -DLLVM_EXTERNAL_LIT=../build/bin/llvm-lit && \
cmake --build .
