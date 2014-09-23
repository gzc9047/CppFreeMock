// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// This file contains internal macros, used to create a uniq mocker for every function, even they have same type.

#ifndef CPP_FREE_MOCK_INTERNAL_MACRO_H_
#define CPP_FREE_MOCK_INTERNAL_MACRO_H_

#include "impl.h"

#define MOCKER_INTERNAL(function, identity) \
    ::CppFreeMock::MockerCreator::GetMocker<::CppFreeMock::TypeForUniqMocker<identity>>(function, #function)

#endif // CPP_FREE_MOCK_INTERNAL_MACRO_H_
