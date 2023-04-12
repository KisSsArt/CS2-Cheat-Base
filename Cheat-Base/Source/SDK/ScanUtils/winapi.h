#pragma once

#include <string>
#include <intrin.h>
#include <windows.h>

#include "nt.h"

#include "../../../Dependencies/fnv.h"

namespace winapi
{
    using module_t = void*;

    class impl {
    public:
        winapi::module_t get_module_address(const hash32_t& module_hash);

        template< typename T >
        __forceinline T get_exported_function(const hash32_t& module_hash, const hash32_t& export_hash)
        {
            const auto module_base = get_module_address(module_hash);
            if (!module_base)
                return NULL;

            auto dos_headers = (PIMAGE_DOS_HEADER)module_base;
            auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)module_base + dos_headers->e_lfanew);
            if (nt_headers->Signature != IMAGE_NT_SIGNATURE)
                return NULL;

            auto export_directory = (PIMAGE_EXPORT_DIRECTORY)((std::uint8_t*)module_base + nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

            for (auto i = 0; i < export_directory->NumberOfNames; i++) {
                auto name = (std::uint8_t*)module_base + ((std::uint32_t*)((std::uint8_t*)module_base + export_directory->AddressOfNames))[i];
                if (fnv::hash(reinterpret_cast<const char*>(name)) == export_hash)
                    return reinterpret_cast<T>((std::uint8_t*)module_base + ((std::uint32_t*)((std::uint8_t*)module_base + export_directory->AddressOfFunctions))[i]);
            }
            return NULL;
        }

        std::string convert_to_multibyte(wchar_t* buffer);
    };
}// namespace winapi

inline winapi::impl g_winapi{ };
