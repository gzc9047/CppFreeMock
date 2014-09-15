// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Hello world.

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// I don't include gmock.h in my code, so please make sure that it included before mine.
#include "cpp_free_mock.h"

#include <iostream>

using namespace ::std;
using namespace ::testing;

string func() {
    return "Non mocked.";
}

TEST(HelloWorld, First) {
    EXPECT_CALL(*MOCKER(func), MOCK_FUNCTION())
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func());
}
