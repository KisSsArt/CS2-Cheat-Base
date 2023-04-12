#include "winapi.h"

namespace utils
{
    template< typename T >
    static __forceinline T get_peb()
    {
        return reinterpret_cast<T>(__readgsqword(0x60));
    }
}// namespace utils

winapi::module_t winapi::impl::get_module_address(const hash32_t& module_hash)
{
    auto peb = utils::get_peb<ntapi::PEB*>();
    if (!peb)
        return nullptr;

    auto list_head = &peb->LoaderData->InLoadOrderModuleList;
    if (!list_head)
        return nullptr;

    winapi::module_t return_module = nullptr;

    for (auto it = list_head->Flink; list_head->Flink != list_head; it = it->Flink) {
        ntapi::PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(it, ntapi::LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        if (!entry->BaseDllName.Buffer)
            continue;

        auto buffer = convert_to_multibyte(entry->BaseDllName.Buffer);

        if (fnv::hash(buffer.data()) == module_hash) {
            return_module = entry->DllBase;
            break;
        }
    }
    return return_module;
}

std::string winapi::impl::convert_to_multibyte(wchar_t* buffer)
{
    auto wstring = std::wstring(buffer);
    if (wstring.empty())
        return "";

    return std::string(wstring.begin(), wstring.end());
}