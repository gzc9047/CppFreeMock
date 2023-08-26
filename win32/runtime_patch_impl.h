#ifndef CPP_FREE_MOCK_WIN32_RUNTIME_PATCH_IMPL_H
#define CPP_FREE_MOCK_WIN32_RUNTIME_PATCH_IMPL_H

#include <cerrno>

#include <unistd.h>

#ifdef WIN32
#include <Windows.h>
#include "mman_win32.h"
#endif

namespace CppFreeMock {

namespace RuntimePatcherImpl {

#ifdef WIN32
static size_t getpagesize() {
    SYSTEM_INFO sSysInfo;
    GetSystemInfo(&sSysInfo);
    return sSysInfo.dwPageSize;
}
#endif

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


#endif // CPP_FREE_MOCK_WIN32_RUNTIME_PATCH_IMPL_H
