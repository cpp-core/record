cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

find_package(Threads REQUIRED)

set(TESTS
  record/basic
  )

set(LIBRARIES
  record
  GTest::gtest
  Threads::Threads
  )

configure_tests("core" "${LIBRARIES}" ${TESTS})
