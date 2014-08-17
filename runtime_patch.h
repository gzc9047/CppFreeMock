// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Check os and include the special implement.

#ifndef CPP_FREE_MOCK_RUNTIME_PATCH_H_
#define CPP_FREE_MOCK_RUNTIME_PATCH_H_

#ifdef __APPLE__
#include "posix/runtime_patch.h"
#elif __linux__
#include "posix/runtime_patch.h"
#endif

#endif // CPP_FREE_MOCK_RUNTIME_PATCH_H_
