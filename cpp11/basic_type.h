// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Basic template type, must be specialization.

#ifndef CPP_FREE_MOCK_CPP11_BASIC_TYPE_H_
#define CPP_FREE_MOCK_CPP11_BASIC_TYPE_H_

namespace CppFreeMock {

template < int uniq >
struct TypeForUniqMocker {
};

template < typename T >
struct MockerBase {
};

template < typename T >
struct Mocker : public MockerBase<T> {
};

template < typename T >
struct MockerWithThisPointerCheck : public MockerBase<T> {
};

template < typename T >
struct MockerStore {
};

template < typename T >
struct MockerStoreWithThisPointer {
};

template < typename T >
struct MockerEntryPoint {
};

template < typename T >
struct MockEntryPointWithThisPointer {
};

} // namespace CppFreeMock

#endif // CPP_FREE_MOCK_CPP11_BASIC_TYPE_H_
