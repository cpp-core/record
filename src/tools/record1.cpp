// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include <iterator>
#include "core/record/iterator.h"

using namespace core;

int main(int argc, const char *argv[]) {
    using RecUInt64FF = record::Iterator<uint64_t, false, false>;
    using RecUInt64FT = record::Iterator<uint64_t, false, true>;
    using RecUInt64TF = record::Iterator<uint64_t, true, false>;
    using RecUInt64TT = record::Iterator<uint64_t, true, true>;

    static_assert(std::random_access_iterator<RecUInt64FF>);
    static_assert(std::random_access_iterator<RecUInt64FT>);
    static_assert(std::random_access_iterator<RecUInt64TF>);
    static_assert(std::random_access_iterator<RecUInt64TT>);
    return 0;
}
