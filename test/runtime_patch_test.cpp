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

using namespace ::std;
using namespace ::testing;
using namespace ::CppFreeMock;

TEST(RuntimePatcher, AlignAddress) {
    cout << "PageSize: " << RuntimePatcher::PageSize << endl;
    for (size_t i = 0; i < RuntimePatcher::PageSize; ++i) {
        EXPECT_EQ(0, RuntimePatcher::AlignAddress(i, RuntimePatcher::PageSize));
    }
}

TEST(RuntimePatcher, mprotect) {
    CREATE_MOCKER(mprotectMocker, mprotect);
    EXPECT_CALL(*mprotectMocker, MOCK_FUNCTION(nullptr, 0, 0))
        .Times(Exactly(1))
        .WillOnce(Return(9047));
    EXPECT_EQ(9047, mprotect(nullptr, 0, 0));
}

TEST(RuntimePatcher, UnprotectMemory) {
    CREATE_MOCKER(alignAddressMocker, RuntimePatcher::AlignAddress);
    void* address = nullptr;
    size_t length = 0;
    EXPECT_CALL(*alignAddressMocker, MOCK_FUNCTION(0, RuntimePatcher::PageSize))
        .Times(Exactly(1))
        .WillOnce(Return(0));
    EXPECT_EQ(0, RuntimePatcher::UnprotectMemory(address, length));
}

TEST(RuntimePatcher, UnprotectMemoryForOnePage) {
    CREATE_MOCKER(unprotectMemoryMocker, RuntimePatcher::UnprotectMemory);
    EXPECT_CALL(*unprotectMemoryMocker, MOCK_FUNCTION(nullptr, RuntimePatcher::PageSize))
        .Times(Exactly(1))
        .WillOnce(Return(9047));
    EXPECT_EQ(9047, RuntimePatcher::UnprotectMemoryForOnePage(nullptr));
}

TEST(RuntimePatcher, IsDistanceOverflow) {
    for (long long i = INT32_MIN; i <= INT32_MAX; i += 0x10000000) {
        EXPECT_FALSE(RuntimePatcher::IsDistanceOverflow(i));
    }
    EXPECT_FALSE(RuntimePatcher::IsDistanceOverflow(INT32_MAX));
    EXPECT_TRUE(RuntimePatcher::IsDistanceOverflow(INT64_MIN));
    EXPECT_TRUE(RuntimePatcher::IsDistanceOverflow(INT64_MAX));
}

TEST(RuntimePatcher, SetJump64bitAddress) {
    char data[16];
    void* function = reinterpret_cast<void*>(data);
    CREATE_MOCKER(patchFunction64bitAddressMocker, RuntimePatcher::PatchFunction64bitAddress);
    CREATE_MOCKER(calculateDistanceMocker, RuntimePatcher::CalculateDistance);
    EXPECT_CALL(*calculateDistanceMocker, MOCK_FUNCTION(function, nullptr))
        .Times(Exactly(1))
        .WillOnce(Return(INT64_MAX));
    EXPECT_CALL(*patchFunction64bitAddressMocker, MOCK_FUNCTION(reinterpret_cast<char*>(function), nullptr))
        .Times(Exactly(1));
    vector<char> binary_backup;
    EXPECT_EQ(0, RuntimePatcher::SetJump(function, nullptr, binary_backup));
}

TEST(RuntimePatcher, SetJump) {
    char data[16];
    char data2[16];
    void* function = reinterpret_cast<void*>(data);
    void* destination = reinterpret_cast<void*>(data2);
    CREATE_MOCKER(patchFunction64bitAddressMocker, RuntimePatcher::PatchFunction64bitAddress);
    CREATE_MOCKER(patchFunctionMocker, RuntimePatcher::PatchFunction);
    EXPECT_CALL(*patchFunction64bitAddressMocker, MOCK_FUNCTION(_, _))
        .Times(Exactly(0));
    EXPECT_CALL(*patchFunctionMocker, MOCK_FUNCTION(reinterpret_cast<char*>(function), _))
        .Times(Exactly(1));
    vector<char> binary_backup;
    EXPECT_EQ(0, RuntimePatcher::SetJump(function, destination, binary_backup));
}
