[![Build](https://github.com/cpp-core/record/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/record/actions/workflows/build.yaml)

## Build

    git clone git@github.com:cpp-core/record
    mkdir record/build && cd record/build
    CC=clang-15 CXX=clang++-15 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
    make check
