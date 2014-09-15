// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// RuntimePatcher test.

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "runtime_patch.h"

// Use CppFreeMock to mock some global function to test itself.
#include "cpp_free_mock.h"

#include <iostream>
#include <vector>

using namespace ::std;
using namespace ::testing;
using namespace ::CppFreeMock;

struct TestRuntimePatcher : public ::testing::Test {
    virtual void TearDown() {
        CLEAR_MOCKER();
    }
};

TEST_F(TestRuntimePatcher, GraftFunctionFailed) {
    auto abortMocker = MOCKER(std::abort);
    EXPECT_CALL(*abortMocker, MOCK_FUNCTION())
        .Times(Exactly(1));
    auto unprotectMemoryForOnePageMocker = MOCKER(RuntimePatcherImpl::UnprotectMemoryForOnePage);
    EXPECT_CALL(*unprotectMemoryForOnePageMocker, MOCK_FUNCTION(_))
        .Times(Exactly(1))
        .WillOnce(Return(6));
    vector<char> binary_backup;
    char data[16];
    char data2[16];
    RuntimePatcher::GraftFunction(data, data2, binary_backup);
}

TEST_F(TestRuntimePatcher, GraftFunctionSuccess) {
    auto abortMocker = MOCKER(std::abort);
    EXPECT_CALL(*abortMocker, MOCK_FUNCTION())
        .Times(Exactly(0));
    auto setJumpMocker = MOCKER(RuntimePatcherImpl::SetJump);
    EXPECT_CALL(*setJumpMocker, MOCK_FUNCTION(_, _, _))
        .Times(Exactly(1))
        .WillOnce(Return(0));
    vector<char> binary_backup;
    char data[16];
    char data2[16];
    RuntimePatcher::GraftFunction(data, data2, binary_backup);
}
