// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Test mock the implement of virtual function, which don't have "virtual" of its declare.

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "cpp_free_mock.h"

#include <functional>
#include <iostream>

using namespace ::std;
using namespace ::testing;

struct BaseClass {
    virtual string func0Parameter() {
        return "BaseClass.";
    }
};

struct ImplementClass : public BaseClass {
    string func0Parameter() {
        return "ImplementClass";
    }
};

struct TestVirtualFunction : public ::testing::Test {
    virtual void TearDown() {
        CLEAR_MOCKER();
    }
};

TEST_F(TestVirtualFunction, Normal) {
    ImplementClass a;
    EXPECT_CALL(*MOCKER(&ImplementClass::func0Parameter), MOCK_FUNCTION(&a))
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    BaseClass* b = &a;
    EXPECT_EQ("Hello world.", b->func0Parameter());
}
