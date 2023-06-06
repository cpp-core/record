// Copyright (C) 2019, 2023 by Mark Melton
//

#include <iterator>
#include <gtest/gtest.h>
#include "core/record/iterator.h"

using namespace core;

TEST(Iterator, IteratorInterface)
{
    using RecUInt64FF = record::Iterator<uint64_t, false, false>;
    using RecUInt64FT = record::Iterator<uint64_t, false, true>;
    using RecUInt64TF = record::Iterator<uint64_t, true, false>;
    using RecUInt64TT = record::Iterator<uint64_t, true, true>;

    static_assert(std::random_access_iterator<RecUInt64FF>);
    static_assert(std::random_access_iterator<RecUInt64FT>);
    static_assert(std::random_access_iterator<RecUInt64TF>);
    static_assert(std::random_access_iterator<RecUInt64TT>);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
