#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>
#include <Psapi.h>

typedef void* (*InstantiateInterfaceFn)();

// Used internally to register classes.
struct InterfaceReg {
    InstantiateInterfaceFn m_CreateFn;
    const char* m_pName;
    InterfaceReg* m_pNext;
};

struct UTILPtr {
public:
    template <typename T>
    UTILPtr(const T& val) {
        m_val = (uintptr_t)(val);
    }

    template <typename T>
    T get() {
        return (T)(m_val);
    }

    UTILPtr& addOffset(int offset) {
        if (m_val) m_val += offset;
        return *this;
    }
    UTILPtr& toAbsolute(int preOffset, int postOffset) {
        if (m_val) {
            addOffset(preOffset);
            m_val = m_val + sizeof(int) + *(int*)(m_val);
            addOffset(postOffset);
        }
        return *this;
    }
    UTILPtr& dereference(int dereferences) {
        if (m_val)
            while (dereferences-- != 0) m_val = *(uintptr_t*)(m_val);
        return *this;
    }

private:
    uintptr_t m_val;
};

class CModule {
public:
    CModule() {};
    CModule(CModule&&) = delete;
    CModule(const CModule&) = delete;

    explicit CModule(const char* name) {
        this->m_name = name;
        this->Load();
    }
    void init(const char* name)
    {
        this->m_name = name;
        this->Load();
    }
    void Load() {
        this->InitializeHandle();
        this->InitializeBounds();
    }

    template <typename T = void*>
    auto GetProcAddress(const char* procName) const {
        T rv = nullptr;
        if (this->IsLoaded()) {
            rv = reinterpret_cast<T>(::GetProcAddress(static_cast<HMODULE>(this->m_handle), procName));
        }
        return rv;
    }
    template <typename T = void*>
    T FindInterfaceAs(const char* version) const {
        void* rv = nullptr;
        if (this->IsLoaded()) {
            UTILPtr pCreateInterface = this->GetProcAddress("CreateInterface");
            InterfaceReg* s_pInterfaceRegs = pCreateInterface.toAbsolute(3, 0).dereference(1).get<InterfaceReg*>();

            for (; s_pInterfaceRegs;
                s_pInterfaceRegs = s_pInterfaceRegs->m_pNext) {
                if (strcmp(version, s_pInterfaceRegs->m_pName) == 0) {
                    rv = s_pInterfaceRegs->m_CreateFn();
                    break;
                }
            }
        }
        return (T)(rv);
    }
    template <size_t N>
    UTILPtr FindPattern(const std::array<int, N>& signature) const {
        UTILPtr rv = 0;
        if (this->IsLoaded()) {
            const int* pSigData = signature.data();
            uint8_t* pBytes = reinterpret_cast<uint8_t*>(this->m_start);
            for (size_t i = 0; i < this->m_end - N; ++i) {
                bool found = true;
                for (size_t j = 0; j < N; ++j) {
                    if (pBytes[i + j] != pSigData[j] && pSigData[j] != -1) {
                        found = false;
                        break;
                    }
                }

                if (found) {
                    rv = reinterpret_cast<uintptr_t>(&pBytes[i]);
                    break;
                }
            }
        }
        return rv;
    }
    const char* GetName() const { return this->m_name.c_str(); }
    bool IsLoaded() const { return this->m_handle != 0; }

private:
    void InitializeHandle() {
        this->m_handle = static_cast<void*>(GetModuleHandle(this->GetName()));
    }
    void InitializeBounds() {
        if (!this->IsLoaded()) 
            return;

        MODULEINFO mi;
        BOOL status = GetModuleInformation(GetCurrentProcess(), static_cast<HMODULE>(this->m_handle), &mi, sizeof(mi));
        if (status != 0) {
            this->m_start = reinterpret_cast<uintptr_t>(this->m_handle);
            this->m_end = static_cast<uintptr_t>(mi.SizeOfImage);
        }
    }

    void* m_handle = nullptr;
    uintptr_t m_start = 0, m_end = 0;
    std::string m_name = "";
};