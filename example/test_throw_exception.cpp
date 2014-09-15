// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Test throw exception in mocker function.

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "cpp_free_mock.h"

#include <functional>
#include <iostream>

using namespace ::std;
using namespace ::testing;

string func0() {
    return "Non mocked.";
}

// throw() make compiler don't generate code for catch.
string func0NoException() noexcept {
    return "Non mocked.";
}

void func1() {
    func0NoException();
    abort();
}

TEST(TestThrowException, ThrowBasicType) {
    auto mocker = MOCKER(func0);
    EXPECT_CALL(*mocker, MOCK_FUNCTION())
        .Times(Exactly(2))
        .WillOnce(Throw(0))
        .WillOnce(Throw(""));
    EXPECT_THROW(func0(), int);
    try {
        func0();
        abort();
    } catch (const char*) {}
    mocker->RestoreToReal();
    EXPECT_EQ("Non mocked.", func0());
}

TEST(TestThrowException, ThrowFromNoExceptionFunction) {
    auto mocker = MOCKER(func0NoException);
    EXPECT_CALL(*mocker, MOCK_FUNCTION())
        .Times(Exactly(1))
        .WillOnce(Throw(0));
    EXPECT_THROW(func1(), int);
}
