// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Test other library's function, make sure that we can only see the declare not definition here.
// This test base on some standard library, so if gmock or gtest call them, this test may fail.
// But I choose them to show that what this tool can do.

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "cpp_free_mock.h"

#include <cerrno>
#include <cstdlib>
#include <iostream>

using namespace ::std;
using namespace ::testing;

TEST(TestOtherLibraryFunction, strerror) {
    char fakeReturn[] = "Hello world.";
    auto mocker = MOCKER(strerror);
    EXPECT_CALL(*mocker, MOCK_FUNCTION(0))
        .Times(Exactly(1))
        .WillOnce(Return(fakeReturn));
    EXPECT_EQ(fakeReturn, strerror(0));
    mocker->RestoreToReal();
    EXPECT_NE(fakeReturn, strerror(0));
}

TEST(TestOtherLibraryFunction, atoi) {
    auto mocker = MOCKER(atoi);
    EXPECT_CALL(*mocker, MOCK_FUNCTION(_))
        .Times(Exactly(1))
        .WillOnce(Return(0));
    EXPECT_EQ(0, atoi("1234567890"));
}
