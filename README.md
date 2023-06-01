[![Build](https://github.com/cpp-core/record/actions/workflows/build.yaml/badge.svg)](https://github.com/cpp-core/record/actions/workflows/build.yaml)

`core::record::Iterator` is an iterator for a contiguous sequence of
equal length records with a run-time determined layout.

## Example

```c++
#include <iostream>
#include <random>
#include <vector>
#include "core/record/record.h"

using namespace core;

void output_records(const std::vector<int>& data, int nrows, int ncols) {
    for (auto i = 0; i < nrows; ++i) {
	const int *row = &data[i * ncols];
	for (auto j = 0; j < ncols; ++j)
	    std::cout << row[j] << " ";
	std::cout << std::endl;
    }
}

int main(int argc, const char *argv[]) {
    // Generate 10k records each with 7 integer numbered sequentially.
    int nrows = 9, ncols = 5;
    std::vector<int> data(nrows * ncols);
    std::generate(data.begin(), data.end(), [n=0]() mutable { return n++; });
    std::cout << std::endl << "sequential: " << std::endl;
    output_records(data, nrows, ncols);

    // Shuffle the records
    std::shuffle(record::begin(data, ncols), record::end(data, ncols), std::mt19937_64{});
    std::cout << std::endl << "shuffled: " << std::endl;
    output_records(data, nrows, ncols);
    
    // Sort the records
    record::Iterator begin(data.data(), ncols);
    record::Iterator end(begin + nrows);
    std::sort(begin, end, [](const int *a, const int *b) {
	return a[0] < b[0];
    });
    std::cout << std::endl << "sorted: " << std::endl;
    output_records(data, nrows, ncols);
    
    return 0;
}
```

## Build

    git clone git@github.com:cpp-core/record
    mkdir record/build && cd record/build
    CC=clang-15 CXX=clang++-15 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
    make check
