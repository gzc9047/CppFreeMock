// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// This file contains internal level symbols. Non relevant with platform.

#ifndef CPP_FREE_MOCK_IMPL_H
#define CPP_FREE_MOCK_IMPL_H

// The struct identity to make sure every mock have different type, we use decltype(mocker)::pMocker internal.
#define CreateMockerWithIdentity(mocker, function, identity, creator) \
    struct identity {}; \
    auto mocker = ::CppFreeMock::MockerCreator<identity>::creator(function)

#define CreateMockerWithInternal2(mocker, function, identity, creator) \
    CreateMockerWithIdentity(mocker, function, FakeTypeForIdentityFunction##identity, creator)

#define CreateMockerWithInternal(mocker, function, identity, creator) \
    CreateMockerWithInternal2(mocker, function, identity, creator)

#endif // CPP_FREE_MOCK_IMPL_H
