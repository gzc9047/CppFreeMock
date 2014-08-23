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

struct ClassA {
    int func0() {
        return 0 + a;
    }
    int a;
};

struct ClassB : ClassA {
    int func0() {
        return 1 + b;
    }
    int b;
};

struct ClassC : ClassB {
    int func0() {
        return 2 + c;
    }
    int c;
};

struct ClassD {
    int func0() {
        return 3 + d;
    }
    int d;
};

struct ClassE : ClassC, ClassD {
    int func0() {
        return 4 + e;
    }
    int e;
};

TEST(TestLargeInheritHierarchy, ClassABC) {
    CREATE_MOCKER(mockerA, &ClassA::func0);
    CREATE_MOCKER(mockerB, &ClassB::func0);
    CREATE_MOCKER(mockerC, &ClassC::func0);
    ClassA classA;
    ClassB classB;
    ClassC classC;
    EXPECT_CALL(*mockerA, MOCK_FUNCTION(&classA))
        .Times(Exactly(1))
        .WillOnce(Return(9045));
    EXPECT_CALL(*mockerA, MOCK_FUNCTION(&classB))
        .Times(Exactly(1))
        .WillOnce(Return(9046));
    EXPECT_CALL(*mockerA, MOCK_FUNCTION(&classC))
        .Times(Exactly(1))
        .WillOnce(Return(9047));
    EXPECT_CALL(*mockerB, MOCK_FUNCTION(&classB))
        .Times(Exactly(1))
        .WillOnce(Return(9048));
    EXPECT_CALL(*mockerB, MOCK_FUNCTION(&classC))
        .Times(Exactly(1))
        .WillOnce(Return(9049));
    EXPECT_CALL(*mockerC, MOCK_FUNCTION(&classC))
        .Times(Exactly(1))
        .WillOnce(Return(9050));
    EXPECT_EQ(9045, classA.ClassA::func0());
    EXPECT_EQ(9046, classB.ClassA::func0());
    EXPECT_EQ(9047, classC.ClassA::func0());
    EXPECT_EQ(9048, classB.ClassB::func0());
    EXPECT_EQ(9049, classC.ClassB::func0());
    EXPECT_EQ(9050, classC.ClassC::func0());
}

TEST(TestLargeInheritHierarchy, ClassE) {
    CREATE_MOCKER(mockerA, &ClassA::func0);
    CREATE_MOCKER(mockerB, &ClassB::func0);
    CREATE_MOCKER(mockerC, &ClassC::func0);
    CREATE_MOCKER(mockerD, &ClassD::func0);
    CREATE_MOCKER(mockerE, &ClassE::func0);
    ClassE classE;
    EXPECT_CALL(*mockerA, MOCK_FUNCTION(&classE))
        .Times(Exactly(1))
        .WillOnce(Return(9045));
    EXPECT_CALL(*mockerB, MOCK_FUNCTION(&classE))
        .Times(Exactly(1))
        .WillOnce(Return(9046));
    EXPECT_CALL(*mockerC, MOCK_FUNCTION(&classE))
        .Times(Exactly(1))
        .WillOnce(Return(9047));
    EXPECT_CALL(*mockerD, MOCK_FUNCTION(static_cast<ClassD*>(&classE)))
        .Times(Exactly(1))
        .WillOnce(Return(9048));
    EXPECT_CALL(*mockerE, MOCK_FUNCTION(static_cast<ClassE*>(&classE)))
        .Times(Exactly(1))
        .WillOnce(Return(9049));
    EXPECT_EQ(9045, classE.ClassA::func0());
    EXPECT_EQ(9046, classE.ClassB::func0());
    EXPECT_EQ(9047, classE.ClassC::func0());
    EXPECT_EQ(9048, classE.ClassD::func0());
    EXPECT_EQ(9049, classE.ClassE::func0());
}
