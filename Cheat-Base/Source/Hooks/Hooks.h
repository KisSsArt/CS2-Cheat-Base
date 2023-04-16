#pragma once

#include <Windows.h>
#include <memory>
#include <d3d11.h>

#include "MinHook/MinHook.h"

VOID WINAPI OnAttach();

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* Resizebuffers) (IDXGISwapChain* thisptr, UINT buffer, UINT w, UINT h, DXGI_FORMAT format, UINT flags);

class Hooks {
public:
    Hooks() noexcept { install(); }
    void install() noexcept;

    MinHook createMove;
    MinHook levelInit;
    MinHook frameStage;
    MinHook mouseInputEnabled;
};

inline std::unique_ptr<Hooks> hooks;