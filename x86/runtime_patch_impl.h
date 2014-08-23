// Copyright 2014 Louix Gu
// Author: gzc9047@gmail.com (Louix Gu)

// CppFreeMock: a tool for mock global function, member function, class static function.
//
// Implement runtime patch in x86 architecture.

#ifndef CPP_FREE_MOCK_X86_RUNTIME_PATCH_IMPL_H_
#define CPP_FREE_MOCK_X86_RUNTIME_PATCH_IMPL_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace CppFreeMock {

namespace RuntimePatcherImpl {
    static std::size_t AlignAddress(const std::size_t address, const std::size_t page_size) {
        return address & (~(page_size - 1));
    }

    static void BackupBinary(const char* const function, std::vector<char>& binary_backup, const std::size_t size) {
        binary_backup = std::vector<char>(function, function + size);
    }

    static bool IsDistanceOverflow(const std::size_t distance) {
        return distance > INT32_MAX && distance < ((long long)INT32_MIN);
    }

    static std::size_t CalculateDistance(const void* const address, const void* const destination) {
        std::size_t distance = reinterpret_cast<std::size_t>(destination)
            - reinterpret_cast<std::size_t>(address)
            - 5; // For jmp instruction;
        return distance;
    }

    // PatchFunction32bitDistance and PatchFunction64bitAddress only support x86 platform.
    static void PatchFunction32bitDistance(char* const function, const std::size_t distance) {
        const char* const distance_bytes = reinterpret_cast<const char*>(&distance);
        // instruction: jmp 0x********;
        function[0] = 0xE9; // jmp
        std::copy(distance_bytes, distance_bytes + 4, function + 1);
    }

    static void PatchFunction64bitAddress(char* const function, const void* const destination) {
        const char* const distance_bytes = reinterpret_cast<const char*>(&destination);
        // instructions:
        // push (low 32bit)
        // mov [rsp + 4] (high 32bit)
        // ret
        function[0] = 0x68; // push
        std::copy(distance_bytes, distance_bytes + 4, function + 1);
        // C7442404 is prefix of: mov dword [rsp+4], *
        function[5] = 0xC7;
        function[6] = 0x44;
        function[7] = 0x24;
        function[8] = 0x04;
        std::copy(distance_bytes + 4, distance_bytes + 8, function + 9);
        function[13] = 0xC3; // ret
    }

    static int SetJump(void* const address, const void* const destination, std::vector<char>& binary_backup) {
        char* const function = reinterpret_cast<char*>(address);
        std::size_t distance = CalculateDistance(address, destination);
        if (IsDistanceOverflow(distance)) {
            BackupBinary(function, binary_backup, 14); // long jmp.
            PatchFunction64bitAddress(function, destination);
        } else {
            // long jmp can cover all the case to patch function, but it need change more binary then this.
            // So I choose to keep this implement.
            // And we should only backup what we change, no more byte.
            BackupBinary(function, binary_backup, 5); // short jmp.
            PatchFunction32bitDistance(function, distance);
        }
        return 0;
    }

    static void RevertJump(void* address, const std::vector<char>& binary_backup) {
        std::copy(binary_backup.begin(), binary_backup.end(), reinterpret_cast<char*>(address));
    }
}

} // namespace CppFreeMock

#endif // CPP_FREE_MOCK_X86_RUNTIME_PATCH_IMPL_H_
