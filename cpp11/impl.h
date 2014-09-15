// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Implement under c++11.

#ifndef CPP_FREE_MOCK_CPP11_IMPL_H_
#define CPP_FREE_MOCK_CPP11_IMPL_H_

#include "cpp11/basic_type.h"

#include "runtime_patch.h"

#include <list>
#include <memory>
#include <unordered_map>
#include <vector>
// I don't include gmock and gtest file here, because maybe you have you own include patch.

namespace CppFreeMock {

// How it works:
//  1, I create a entry point for runtime patch, to move the cpu run here;
//  2, In the entry point, it call a mocked method to do the mock function.
//      a) Because the entry point is a static function, so the mocked method should be static or invoke from a static variable. 
//      b) Because we can mock member function, so the mocked method must be a member function, so we can only use a static varibale to call it.
//  3, So I create MockerStore, with a static variable.
//  4, Hack gmock, create Mocker as MOCK_METHOD does.

// I use function type to combine one type and a few other type into one type.
// We can also use another type like Combiner to do this.
// In this file:
//  1, type I means the uniq id type for every mocker.
//  2, type C means class type of the mockee function, if it's a member function of static function.
//  3, type R means the mockee function's return type.
//  4, variable type ... P means the function's parameter types.

// Here F means the mockee function type.
template < typename I, typename F >
struct MockerStore<I(F)> {
    static Mocker<I(F)>* pMocker;
};

template < typename I, typename F >
struct MockerStoreWithThisPointer<I(F)> {
    static MockerWithThisPointerCheck<I(F)>* pMocker;
};

template < typename I, typename F > Mocker<I(F)>* MockerStore<I(F)>::pMocker = nullptr;
template < typename I, typename F > MockerWithThisPointerCheck<I(F)>* MockerStoreWithThisPointer<I(F)>::pMocker = nullptr;

template < typename I, typename R, typename ... P >
struct MockerEntryPoint<I(R(P ...))> {
    static R EntryPoint(P... p) {
        return MockerStore<I(R(P ...))>::pMocker->CppFreeMockStubFunction(p ...);
    }
};

#define MOCKER_ENTRY_POINT_WITH_THIS_POINTER(constness) \
template < typename I, typename C, typename R, typename ... P > \
struct MockEntryPointWithThisPointer<I(R(C::*)(P ...) constness)> { \
    R EntryPoint(P... p) { \
        return MockerStoreWithThisPointer<I(R(C::*)(const void*, P ...) constness)>::pMocker->CppFreeMockStubFunction(this, p ...); \
    } \
}

MOCKER_ENTRY_POINT_WITH_THIS_POINTER(const);
MOCKER_ENTRY_POINT_WITH_THIS_POINTER();

#undef MOCKER_ENTRY_POINT_WITH_THIS_POINTER

// This used to map MockerBase::CppFreeMockStubFunction's parameter to gmock's parameter, till now no need to use it.
// If we use it, wo need a parameters mapper to map Type A,B,... => Type Matcher<A>,Matcher<B>,... as the parameter.
// Example: https://github.com/gzc9047/cpp_non_virtual_mock/blob/master/test_type_mapper.cpp
template < typename T >
struct GmockMatcherMapper {
    typedef const ::testing::Matcher<T>& Type;
};

// This class is hack how gmock implement it.
template < typename R, typename ... P >
struct MockerBase<R(P ...)> {
    MockerBase(const std::string& _functionName): functionName(_functionName) {}
    virtual ~MockerBase() {}

    // Use 'CppFreeMockStubFunction' as the function name for EXPECT_CALL.
    R CppFreeMockStubFunction(P... p) {
        gmocker.SetOwnerAndName(this, functionName.c_str());
        return gmocker.Invoke(p ...);
    }

    // Here M means the Matcher type, not the real parameters.
    // For example ::testing::_ can match any value but is not a real parameter type.
    template < typename ... M >
    ::testing::MockSpec<R(P...)>& gmock_CppFreeMockStubFunction(M... m) {
        gmocker.RegisterOwner(this);
        return gmocker.With(m ...);
    }

    virtual void RestoreToReal() = 0;

    mutable ::testing::FunctionMocker<R(P...)> gmocker;
    std::vector<char> binaryBackup; // Backup the mockee's binary code changed in RuntimePatcher.
    const std::string functionName;
};

template < typename I, typename R, typename ... P>
struct Mocker<I(R(P ...))> : MockerBase<R(P ...)> {
    typedef I IntegrateType(R(P ...));
    typedef R FunctionType(P ...);
    Mocker(FunctionType function, const std::string& functionName):
        MockerBase<FunctionType>(functionName),
        originFunction(function) {
        RuntimePatcher::GraftFunction(originFunction,
                MockerEntryPoint<IntegrateType>::EntryPoint,
                MockerBase<FunctionType>::binaryBackup);
        MockerStore<IntegrateType>::pMocker = this;
    }

    virtual ~Mocker() {
        RestoreToReal();
    }

    void RestoreToReal() {
        RuntimePatcher::RevertGraft(originFunction, MockerBase<FunctionType>::binaryBackup);
        MockerStore<IntegrateType>::pMocker = nullptr;
    }

    FunctionType* originFunction;
};

#define MOCKER_WITH_THIS_POINTER_CHECK(constness) \
template < typename I, typename C, typename R, typename ... P> \
struct MockerWithThisPointerCheck<I(R(C::*)(const void*, P ...) constness)> : MockerBase<R(const void*, P ...)> { \
    typedef I IntegrateType(R(C::*)(const void*, P ...) constness); \
    typedef I EntryPointType(R(C::*)(P ...) constness); \
    typedef R (C::*FunctionType)(P ...) constness; \
    typedef R StubFunctionType(const void*, P ...); \
    MockerWithThisPointerCheck(FunctionType function, const std::string& functionName): \
        MockerBase<StubFunctionType>(functionName), \
        originFunction(function) { \
        RuntimePatcher::GraftFunction(originFunction, \
                &MockEntryPointWithThisPointer<EntryPointType>::EntryPoint, \
                MockerBase<StubFunctionType>::binaryBackup); \
        MockerStoreWithThisPointer<IntegrateType>::pMocker = this; \
    } \
    virtual ~MockerWithThisPointerCheck() { \
        RestoreToReal(); \
    } \
    virtual void RestoreToReal() { \
        RuntimePatcher::RevertGraft(originFunction, MockerBase<StubFunctionType>::binaryBackup); \
        MockerStoreWithThisPointer<IntegrateType>::pMocker = nullptr; \
    } \
    FunctionType originFunction; \
}

MOCKER_WITH_THIS_POINTER_CHECK(const);
MOCKER_WITH_THIS_POINTER_CHECK();

#undef MOCKER_WITH_THIS_POINTER_CHECK

template < typename T >
struct SimpleSingleton {
    static T& getInstance() {
        static T value;
        return value;
    }
};

template < typename T >
struct MockerCache {
private:
    friend class MockerCreator;
    typedef std::unordered_map<const void*, const std::shared_ptr<T>> HashMap;

    static HashMap& getInstance() {
        return SimpleSingleton<HashMap>::getInstance();
    }

    static void RestoreCachedMockFunctionToReal() {
        for (auto& mocker : getInstance()) {
            mocker.second->RestoreToReal();
        }
        getInstance().clear();
    }
};

struct MockerCreator {
private:
    typedef std::list<std::function<void()>> RestoreFunction;
#define CREATE_MOCKER_WITH_THIR_POINTER_CHECK(constness) \
    template < typename I, typename C, typename R, typename ... P > \
    static const std::shared_ptr<MockerBase<R(const void*, P ...)>> \
            CreateMocker(R (C::*function)(P ...) constness, const std::string& functionName) { \
        typedef I IntegrateType(R(C::*)(const void*, P ...) constness); \
        return std::shared_ptr<MockerBase<R(const void*, P ...)>>(new MockerWithThisPointerCheck<IntegrateType>(function, functionName)); \
    }

    CREATE_MOCKER_WITH_THIR_POINTER_CHECK(const)
    CREATE_MOCKER_WITH_THIR_POINTER_CHECK()
#undef CREATE_MOCKER_WITH_THIR_POINTER_CHECK

    template < typename I, typename R, typename ... P >
    static const std::shared_ptr<MockerBase<R(P ...)>>
            CreateMocker(R function(P ...), const std::string& functionName) {
        return std::shared_ptr<MockerBase<R(P ...)>>(new Mocker<I(R(P ...))>(function, functionName));
    }

    template < typename I, typename M, typename F >
    static const std::shared_ptr<M>
            DoGetMocker(F function, const std::string& functionName) {
        typedef MockerCache<M> MockerCacheType;
        const void* address = reinterpret_cast<const void*>((std::size_t&)function);
        auto got = MockerCacheType::getInstance().find(address);
        if (got != MockerCacheType::getInstance().end()) {
            return got->second;
        } else {
            SimpleSingleton<RestoreFunction>::getInstance().push_back(std::bind(MockerCacheType::RestoreCachedMockFunctionToReal));
            MockerCacheType::getInstance().insert(std::make_pair(address, CreateMocker<I>(function, functionName)));
            return DoGetMocker<I, M>(function, functionName);
        }
    }

public:
#define GET_MOCKER_WITH_THIR_POINTER_CHECK(constness) \
    template < typename I, typename C, typename R, typename ... P > \
    static std::shared_ptr<MockerBase<R(const void*, P ...)>> \
            GetMocker(R (C::*function)(P ...) constness, const std::string& functionName) { \
        return DoGetMocker<I, MockerBase<R(const void*, P ...)>>(function, functionName); \
    }

    GET_MOCKER_WITH_THIR_POINTER_CHECK(const)
    GET_MOCKER_WITH_THIR_POINTER_CHECK()
#undef GET_MOCKER_WITH_THIR_POINTER_CHECK

    template < typename I, typename R, typename ... P >
    static const std::shared_ptr<MockerBase<R(P ...)>>
            GetMocker(R function(P ...), const std::string& functionName) {
        return DoGetMocker<I, MockerBase<R(P ...)>>(function, functionName);
    }

    static void RestoreAllMockerFunctionToReal() {
        for (auto& restorer : SimpleSingleton<RestoreFunction>::getInstance()) {
            restorer();
        }
        SimpleSingleton<RestoreFunction>::getInstance().clear();
    }
};

} // namespace CppFreeMock

#endif // CPP_FREE_MOCK_CPP11_IMPL_H_
