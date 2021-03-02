#! /usr/bin/env bash

cmake -G Ninja -B build -DMLIR_DIR=../build/lib/cmake/mlir && \
cmake --build build
