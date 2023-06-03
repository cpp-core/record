
# Motivation
The `C++` standard library provides a variety of iterator-based
algorithms along with methods for creating iterators for many types of
containers. However, consider a `std::vector<int>` representing a
seqeunce of records each with `ncols` integer elements where `ncols`
is a run-time parameter. There is no simple way to construct an
iterator for these records.

# Synopsis
The `core::record` library provides standards conforming (as much as
feasible) iterators to fill this role.

{cpp:type}`core::record::Iterator` - an iterator for a contiguous sequence of
equal length records of a possibly opaque type and run-time determined
length.

{cpp:type}`core::record::FixedIterator` - an iterator for a contiguous
sequence of equal length records of a possibly opaque type and
compile-time determined length.

# Basics

## Iterator Example

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

# Build

    git clone git@github.com:cpp-core/record
    mkdir record/build && cd record/build
    CC=clang-15 CXX=clang++-15 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
    make check

# Installation

```bash
git clone https://github.com/cpp-core/pp
mkdir pp/build && cd pp/build
CC=clang-mp-14 CXX=clang++-mp-14 cmake -DCMAKE_INSTALL_PREFIX=$HOME/opt ..
make -j4 check # Run tests
make install   # Do the install
```

To build the documentation (requires doxygen and sphinx-build to be installed)::

```bash
CC=clang-11 CXX=clang++11 cmake -DCORE_DOCS ..
make cxx_core_pp_docs # root of html tree is docs/html/index.html
```

# Background

*Record* is part of the *cpp-core** family of `C++` libraries. The
*cpp-core* libraries have two primary goals:

- **Ergonomics**. In the spirit of the `CppCoreGuideLines`, cpp-core
  aspires to facilitate writing concise, idiomatic code not by force,
  but by providing a clear path that produces robust, easy to read
  code. Good code should be fun to write.
  
- **Components**. Modern language platforms provide broad support for
  first-class library components, while C++ has traditionally only
  provided a minimal standard library. *cpp-core* aspires to
  provide support for a broad range of libraries that feel like part
  of the language.
  
# License

This software is licensed under the BSD 3-clause license. See the LICENSE file for details.

# Appendix

```{toctree}
---
caption: API REFERENCE
---
iterator
```

```{toctree}
---
caption: FILE REFERENCE
maxdepth: 5
---
include/ <generated/include/dir>
test/ <generated/test/dir>
```
