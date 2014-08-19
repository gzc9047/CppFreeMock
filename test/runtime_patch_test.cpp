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

TEST(RuntimePatcher, GraftFunctionFailed) {
    CREATE_MOCKER(abortMocker, std::abort);
    EXPECT_CALL(*abortMocker, MOCK_FUNCTION())
        .Times(Exactly(1));
    CREATE_MOCKER(unprotectMemoryForOnePageMocker, RuntimePatcherImpl::UnprotectMemoryForOnePage);
    EXPECT_CALL(*unprotectMemoryForOnePageMocker, MOCK_FUNCTION(_))
        .Times(Exactly(1))
        .WillOnce(Return(6));
    vector<char> binary_backup;
    char data[16];
    char data2[16];
    RuntimePatcher::GraftFunction(data, data2, binary_backup);
}

TEST(RuntimePatcher, GraftFunctionSuccess) {
    CREATE_MOCKER(abortMocker, std::abort);
    EXPECT_CALL(*abortMocker, MOCK_FUNCTION())
        .Times(Exactly(0));
    CREATE_MOCKER(setJumpMocker, RuntimePatcherImpl::SetJump);
    EXPECT_CALL(*setJumpMocker, MOCK_FUNCTION(_, _, _))
        .Times(Exactly(1))
        .WillOnce(Return(0));
    vector<char> binary_backup;
    char data[16];
    char data2[16];
    RuntimePatcher::GraftFunction(data, data2, binary_backup);
}

TEST(RuntimePatcher, RevertGraft) {
    CREATE_MOCKER(revertJumpMocker, RuntimePatcherImpl::RevertJump);
    EXPECT_CALL(*revertJumpMocker, MOCK_FUNCTION(_, _))
        .Times(Exactly(1));
}
