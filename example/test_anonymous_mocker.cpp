// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Test create anonymous mocker.

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "cpp_free_mock.h"

#include <functional>
#include <iostream>

using namespace ::std;
using namespace ::testing;

string func0Parameter() {
    return "Non mocked.";
}

string func0Parameter2() {
    return "Non mocked.";
}

struct TestMockStruct {
    string func0Parameter() {
        return "Non mocked.";
    }
};

struct TestAnonymousMocker : public ::testing::Test {
    virtual void TearDown() {
        CLEAR_MOCKER();
    }
};

TEST_F(TestAnonymousMocker, TestSimpleSingleton) {
    int a = 0;
    ::CppFreeMock::SimpleSingleton<int*>::getInstance() = &a;
    EXPECT_EQ(&a, ::CppFreeMock::SimpleSingleton<int*>::getInstance());
    *::CppFreeMock::SimpleSingleton<int*>::getInstance() = 2;
    EXPECT_EQ(2, a);
}

TEST_F(TestAnonymousMocker, AnonymousMocker) {
    EXPECT_CALL(*MOCKER(func0Parameter), MOCK_FUNCTION())
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func0Parameter());
}

TEST_F(TestAnonymousMocker, AnonymousMemberMocker) {
    EXPECT_EQ("Non mocked.", TestMockStruct().func0Parameter());
    EXPECT_CALL(*MOCKER(&TestMockStruct::func0Parameter), MOCK_FUNCTION(_))
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", TestMockStruct().func0Parameter());
}

TEST_F(TestAnonymousMocker, NormalMocker) {
    EXPECT_EQ("Non mocked.", func0Parameter());
    auto mocker = MOCKER(func0Parameter);
    EXPECT_CALL(*mocker, MOCK_FUNCTION())
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func0Parameter());
}

TEST_F(TestAnonymousMocker, SameMocker) {
    EXPECT_EQ(MOCKER(func0Parameter), MOCKER(func0Parameter));
    EXPECT_EQ(MOCKER(&TestMockStruct::func0Parameter), MOCKER(&TestMockStruct::func0Parameter));
}
