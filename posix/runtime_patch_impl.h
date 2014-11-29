// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Implement CppFreeMock::RuntimePatcherImpl::UnprotectMemory in posix.

#ifndef CPP_FREE_MOCK_POSIX_RUNTIME_PATCH_IMPL_H_
#define CPP_FREE_MOCK_POSIX_RUNTIME_PATCH_IMPL_H_

#include <cerrno>

#include <unistd.h>

#include <sys/mman.h>

namespace CppFreeMock {

namespace RuntimePatcherImpl {
    // To be simple, I don't create .cpp for this static value, so you can't include this file in 2 or more cpp file.
    static std::size_t PageSize = getpagesize();

    // return 0 for success, otherwire for error.
    static int UnprotectMemory(const void* const address, const size_t length) {
        // Here we assume that page size is power of 2.
        void* const page = reinterpret_cast<void*>(AlignAddress(reinterpret_cast<std::size_t>(address), PageSize));
        int ret = mprotect(page, length, PROT_READ | PROT_WRITE | PROT_EXEC);
        if (ret != 0) {
            int err = errno;
            std::cerr << "Unprotect memory: " << address
                << " meet errno: " << err
                << " description: " << strerror(err) << std::endl;
        }
        return ret;
    }

    // return 0 for success, otherwire for error.
    static int UnprotectMemoryForOnePage(void* const address) {
        return UnprotectMemory(address, PageSize);
    }
}

} // namespace CppFreeMock

#endif // CPP_FREE_MOCK_POSIX_RUNTIME_PATCH_IMPL_H_
