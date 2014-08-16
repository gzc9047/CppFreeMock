// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// This file contains user level macro.
// User can only use the symbols in this file.

#ifndef CPP_FREE_MOCK_H
#define CPP_FREE_MOCK_H

#include "cpp_free_mock_impl.h"

// mocker is the variable name in user code.
#define CREATE_MOCKER(mocker, function) \
    CreateMockerWithInternal(mocker, function, __LINE__, createMockerWithIdentity)

// Used to create a member function mocker that can check this pointer.
#define CREATE_MOCKER_T(mocker, function) \
    CreateMockerWithInternal(mocker, function, __LINE__, createMockerWithIdentityWithThisPointerCheck)

#endif // CPP_FREE_MOCK_H
