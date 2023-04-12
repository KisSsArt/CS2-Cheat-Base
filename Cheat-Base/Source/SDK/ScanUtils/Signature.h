#pragma once

#include <cstdint>
#include <vector>
#include <winnt.h>

#include "winapi.h"


class signature {
public:
    std::uint8_t* m_addr;
public:
    __forceinline signature(std::uint8_t* addr)
    {
        m_addr = addr;
    }

    template< typename T = std::uint8_t* >
    __forceinline T get()
    {
        return T(m_addr);
    }

    __forceinline signature sub(std::uintptr_t bytes)
    {
        return signature(m_addr - bytes);
    }

    __forceinline signature add(std::uintptr_t bytes)
    {
        return signature(m_addr + bytes);
    }

    __forceinline signature deref(int value = 1)
    {
        for (auto i = 0; i < value; ++i)
            m_addr = *reinterpret_cast<std::uint8_t**>(m_addr);

        return signature(m_addr);
    }

    static __declspec(noinline) signature search(const hash32_t& module_name, const char* signature_bytes)
    {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector< int >{ };
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for (auto current = start; current < end; ++current) {
                if (*current == '?') {
                    ++current;

                    if (*current == '?')
                        ++current;

                    bytes.push_back(-1);
                }
                else
                    bytes.push_back(strtoul(current, &current, 16));
            }

            return bytes;
        };

        auto handle = g_winapi.get_module_address(module_name);
        auto dos_headers = (PIMAGE_DOS_HEADER)handle;
        auto headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)handle + dos_headers->e_lfanew);

        auto image_size = headers->OptionalHeader.SizeOfImage;
        auto pattern_bytes = pattern_to_byte(signature_bytes);
        auto scan_bytes = reinterpret_cast<std::uint8_t*>(handle);

        auto s = pattern_bytes.size();
        auto d = pattern_bytes.data();

        for (auto i = 0ul; i < image_size - s; ++i) {
            bool found = true;

            for (auto j = 0ul; j < s; ++j) {
                if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }

            if (found)
                return &scan_bytes[i];
        }

        return signature(0);
    }
};