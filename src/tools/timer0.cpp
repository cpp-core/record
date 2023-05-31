// Copyright (C) 2022, 2023 by Mark Melton
//

#include <atomic>
#include <iomanip>
#include <iostream>
#include "core/timer/timer.h"

using namespace core::timer;
using std::cout, std::endl;

int main(int argc, const char *argv[]) {

    unsigned int output{};
    auto ns = Timer().run(1'000'000, [&]() {
	output += 1;
	doNotOptimizeAway(output);
    }).elapsed_per_iteration();
    cout << std::setprecision(2) << ns << " nanoseconds per operation" << endl;
	
    return 0;
}
