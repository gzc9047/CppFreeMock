// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Test global function:
//  1, multi parameter;
//  2, reference parameter;
//  3, different namespace & same function name;
//  4, rollback to no mock;

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

string func5Parameter(bool, char, int, string, const string&) {
    return "Non mocked.";
}

int func5ReferenceParameter(bool&, char&, int&, string&, const string&) {
    return 0;
}

int testReferenceStubP1P2(bool& boolValue, char& charValue) {
    boolValue = !boolValue;
    charValue += 1;
    return 12;
}

int testReferenceStubP3P4P5(int& intValue, string& stringValue, const string& stringConstValue) {
    --intValue;
    stringValue.resize(0);
    cout << "Get stringConstValue: " << stringConstValue << endl;
    return 345;
}

struct TestGlobalFunction : public ::testing::Test {
    TestGlobalFunction():
        boolValue(false),
        charValue('0'),
        intValue(0),
        stringValue(""),
        stringConstValue("HELLO") { }
    virtual void SetUp() { }
    virtual void TearDown() { }

    bool boolValue;
    char charValue;
    int intValue;
    string stringValue;
    const string stringConstValue;
};

TEST_F(TestGlobalFunction, RestoreToReal) {
    CREATE_MOCKER(mocker, func0Parameter);
    EXPECT_CALL(*mocker, MOCK_FUNCTION())
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", func0Parameter());
    mocker->RestoreToReal();
    EXPECT_EQ("Non mocked.", func0Parameter());
}

TEST_F(TestGlobalFunction, MultiParameter) {
    auto call = bind(func5Parameter, false, '0', 0, "LOUIX", "");
    CREATE_MOCKER(mocker, func5Parameter);
    EXPECT_CALL(*mocker, MOCK_FUNCTION(_, _, _, _, _))
        .Times(Exactly(1))
        .WillOnce(Return("Hello world."));
    EXPECT_EQ("Hello world.", call());
    EXPECT_CALL(*mocker, MOCK_FUNCTION(false, _, 0, _, _))
        .Times(Exactly(1))
        .WillOnce(Return("With false and 0."));
    EXPECT_EQ("With false and 0.", call());
    mocker->RestoreToReal();
    EXPECT_EQ("Non mocked.", call());
}

TEST_F(TestGlobalFunction, MultiReferenceParameter) {
    auto call = bind(func5ReferenceParameter,
            ref(boolValue),
            ref(charValue),
            ref(intValue),
            ref(stringValue),
            ref(stringConstValue));
    CREATE_MOCKER(mocker, func5ReferenceParameter);
    EXPECT_CALL(*mocker, MOCK_FUNCTION(_, _, _, _, _))
        .Times(Exactly(1))
        .WillOnce(Return(1));
    EXPECT_EQ(1, call());
    boolValue = true;
    intValue = 1;
    EXPECT_CALL(*mocker, MOCK_FUNCTION(true, _, 1, _, _))
        .Times(Exactly(2))
        .WillRepeatedly(Return(2));
    EXPECT_EQ(2, func5ReferenceParameter(boolValue, charValue, intValue, stringValue, ""));
    EXPECT_EQ(2, call());
    mocker->RestoreToReal();
    EXPECT_EQ(0, call());
}


TEST_F(TestGlobalFunction, MultiReferenceParameterWithArgsAction) {
    // TODO(guzuchao): impl.
}
