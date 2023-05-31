// Copyright (C) 2023 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/timer/timer.h"

using namespace core::timer;

TEST(Timer, Basic)
{
    Timer t;
    t.start();
    t.stop();
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
