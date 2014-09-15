// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Basic template type, must be specialization.

#ifndef CPP_FREE_MOCK_CPP11_BASIC_TYPE_H_
#define CPP_FREE_MOCK_CPP11_BASIC_TYPE_H_

namespace CppFreeMock {

template < int uniq >
struct TypeForUniqMocker { };

template < typename T >
struct MockerBase { };

template < typename T >
struct Mocker : public MockerBase<T> { };

template < typename T >
class MockerEntryPoint { };

// This used to map MockerBase::CppFreeMockStubFunction's parameter to gmock's parameter, till now no need to use it.
// If we use it, wo need a parameters mapper to map Type A,B,... => Type Matcher<A>,Matcher<B>,... as the parameter.
// Example: https://github.com/gzc9047/cpp_non_virtual_mock/blob/master/test_type_mapper.cpp
template < typename T >
struct GmockMatcherMapper {
    typedef const ::testing::Matcher<T>& Type;
};

template < typename T >
struct SimpleSingleton {
    static T& getInstance() {
        static T value;
        return value;
    }
};

} // namespace CppFreeMock

#endif // CPP_FREE_MOCK_CPP11_BASIC_TYPE_H_
