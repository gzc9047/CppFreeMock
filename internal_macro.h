// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// This file contains internal macros, used to create a uniq mocker for every function, even they have same type.

#ifndef CPP_FREE_MOCK_INTERNAL_MACRO_H_
#define CPP_FREE_MOCK_INTERNAL_MACRO_H_

#include "impl.h"

// The struct identity to make sure every mock have different type, we use decltype(mocker)::pMocker internal.
#define CREATE_MOCKER_UNIQ(mocker, function, uniq, creator) \
    struct uniq {}; \
    auto mocker = ::CppFreeMock::MockerCreator<uniq>::creator(function)

#define CREATE_MOCKER_INTERNAL2(mocker, function, identity, creator) \
    CREATE_MOCKER_UNIQ(mocker, function, TypeForUniqMocker##identity, creator)

#define CREATE_MOCKER_INTERNAL(mocker, function, identity, creator) \
    CREATE_MOCKER_INTERNAL2(mocker, function, identity, creator)

#endif // CPP_FREE_MOCK_INTERNAL_MACRO_H_
