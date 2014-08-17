// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// This file contains user level macro.
// User can only use the symbols in this file.

#ifndef CPP_FREE_MOCK_CPP_FREE_MOCK_H_
#define CPP_FREE_MOCK_CPP_FREE_MOCK_H_

#include "internal_macro.h"

// mocker is the variable name in user code.
#define CREATE_MOCKER(mocker, function) \
    CREATE_MOCKER_INTERNAL(mocker, function, __COUNTER__, CreateMocker)

// Used to create a member function mocker that can check this pointer.
#define CREATE_MOCKER_T(mocker, function) \
    CREATE_MOCKER_INTERNAL(mocker, function, __COUNTER__, CreateMockerWithThisPointerCheck)

// Used in EXPECT_CALL(*mocker, MOCK_FUNCTION(_))
#define MOCK_FUNCTION MockFunction

#endif // CPP_FREE_MOCK_CPP_FREE_MOCK_H_
