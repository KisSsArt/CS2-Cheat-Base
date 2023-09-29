#include "Hooks.h"

#include "../Interfaces.h"
#include "../Memory/Memory.h"
#include "../Menu/Menu.h"
#include "../Config/Config.h"

#include "Source2/InputSystem.h"
#include "Source2/Input.h"
#include "Source2/FrameStage.h"
#include "Source2/GlobalVars.h"
#include "Source2/EngineClient.h"
#include "Source2/GameEntitySystem.h"
#include "Source2/CSPlayerPawnBase.h"
#include "Source2/BasePlayerWeapon.h"
#include "Source2/CPlayer_WeaponServices.h"

#include "Utils.h"
#include "LocalPlayer.h"

#include "../GameData/GameData.h"
#include "../Hacks/Misc.h"
#include "../Hacks/Visuals.h"
#include "../Hacks/ESP.h"

#include <kiero/kiero.h>
#include <kiero/minhook/include/MinHook.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

bool init = false;

HWND window;
WNDPROC originalWndProc;
Present originalPresent;
Resizebuffers originalResizeBuffers;

ID3D11Device* device = NULL;
ID3D11DeviceContext* context = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT __stdcall wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_KEYDOWN && wParam == VK_INSERT)
        menu->open = !menu->open;

    if (msg == WM_KEYDOWN && wParam == VK_ESCAPE && menu->open)
    {
        menu->open = false;
        return true;
    }

    ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);
    interfaces->inputSystem->enableInput(!menu->open);

    if (menu->open)
    {
        switch (msg) {
        case WM_MOUSEMOVE:
        case WM_NCMOUSEMOVE:
        case WM_MOUSELEAVE:
        case WM_NCMOUSELEAVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONDBLCLK:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_CHAR:
        case WM_SETCURSOR:
        case WM_DEVICECHANGE:
            return true;
        }

        if (ImGui::GetIO().WantTextInput || ImGui::GetIO().MouseWheel)
            return true;
    }

    return CallWindowProcW(originalWndProc, window, msg, wParam, lParam);
}

bool __fastcall createMoveHook(void* ecx, int a2, std::uint8_t a3)
{
    static auto original = (bool(__thiscall*)(void*, int, std::uint8_t))hooks->createMove.getDetour();

    auto result = original(ecx, a2, a3);

    auto cmd = memory->input->getUserCmd(ecx, a2);
    if (!cmd)
        return result;

    auto baseCmd = cmd->getBase();
    if (!baseCmd)
        return result;

    if (!localPlayer)
        return result;

    CSPlayerController* localController = memory->entitySystem->getLocalPlayerController();
    if (!localController)
        return result;

    CSPlayerPawnBase* localPawn = localController->getPawn();
    if (!localPawn)
        return result;

    CPlayer_WeaponServices* weaponServices = localPawn->getWeaponServices();
    if (!weaponServices)
        return result;

    BasePlayerWeapon* weapon = weaponServices->getActiveWeapon();
    if (!weapon)
        return result;

    if (menu->open)
        cmd->buttons &= ~(UserCmd::IN_ATTACK | UserCmd::IN_ATTACK2 | UserCmd::IN_BACK | UserCmd::IN_FORWARD | UserCmd::IN_MOVERIGHT | UserCmd::IN_MOVELEFT);

    //your stuff
    Misc::BunnyHop(cmd);

    return result;
}

void __fastcall frameStageNotifyHook(void* ecx, int stage)
{
    static auto original = (void(__thiscall*)(void*, int))hooks->frameStage.getDetour();

    if (stage == FRAME_START)
    {
        GameData::update();
    }

    original(ecx, stage);

    localPlayer.init((interfaces->engineClient->isConnected() && interfaces->engineClient->isInGame()) ? *(BaseEntity***)(memory->entitySystem->getBaseEntity<BaseEntity*>(0)) : nullptr);
}

__int64 __fastcall levelInitHook(void* ecx, __int64 a2)
{
    static auto original = (_int64(__thiscall*)(void*, __int64))hooks->levelInit.getDetour();

    memory->globalVars = memory->clientModule.FindPattern(SIGNATURE("48 8B 05 ? ? ? ? 48 8B D8 80 78 3D 00 75 1D 80 78 3C 00 75")).toAbsolute(3, 7).get<GlobalVars*>();

    return original(ecx, a2);
}

static bool __fastcall mouseInputEnabledHook(void* rcx)
{
    static auto original = (bool(__thiscall*)(void*))hooks->mouseInputEnabled.getDetour();

    if (menu->open)
        return false;

    return original(rcx);
}

HRESULT __stdcall presentHook(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) noexcept
{
    if (!init)
    {
        if (!SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device)))
            return originalPresent(swapChain, syncInterval, flags);

        ID3D11Texture2D* backbuffer;
        if (SUCCEEDED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backbuffer))))
        {
            D3D11_RENDER_TARGET_VIEW_DESC desc{};
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
            device->CreateRenderTargetView(backbuffer, nullptr, &renderTargetView);
            if (backbuffer) { backbuffer->Release(); backbuffer = nullptr; }
        }
        device->GetImmediateContext(&context);

        DXGI_SWAP_CHAIN_DESC sd;
        swapChain->GetDesc(&sd);
        window = sd.OutputWindow;

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(device, context);

        Netvar::init();

        originalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)wndProc);

        init = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //your stuff
    if (ImGui::GetBackgroundDrawList())
    {
        Visuals::NoScopeCrosshair(ImGui::GetBackgroundDrawList());
        ESP::render(ImGui::GetBackgroundDrawList());
    }

    menu->render();

    context->OMSetRenderTargets(1, &renderTargetView, NULL);

    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return originalPresent(swapChain, syncInterval, flags);
}

HRESULT WINAPI resizebuffersHook(IDXGISwapChain* swapChain, UINT buffer, UINT w, UINT h, DXGI_FORMAT format, UINT flags)
{
    auto hr = originalResizeBuffers(swapChain, buffer, w, h, format, flags);

    if (renderTargetView) { renderTargetView->Release(); renderTargetView = nullptr; }

    ImGui_ImplDX11_InvalidateDeviceObjects();

    if (SUCCEEDED(hr))
    {
        swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
        device->GetImmediateContext(&context);

        ID3D11Texture2D* backbuffer;
        if (SUCCEEDED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backbuffer))))
        {
            D3D11_RENDER_TARGET_VIEW_DESC desc{};
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
            device->CreateRenderTargetView(backbuffer, &desc, &renderTargetView);
            if (backbuffer) { backbuffer->Release(); backbuffer = nullptr; }
        }

        ImGui_ImplDX11_CreateDeviceObjects();
    }
    return hr;
}

void Hooks::install() noexcept
{
    interfaces = std::make_unique<const Interfaces>();
    memory = std::make_unique<Memory>();
    config = std::make_unique<Config>();
    menu = std::make_unique<Menu>();

    createMove.detour(memory->createMove, createMoveHook);
    levelInit.detour(memory->levelInit, levelInitHook);
    frameStage.detour(memory->frameStage, frameStageNotifyHook);
    mouseInputEnabled.detour(memory->mouseInputEnabled, mouseInputEnabledHook);

    MH_EnableHook(MH_ALL_HOOKS);
}

VOID WINAPI OnAttach()
{
    while (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
    {
        kiero::bind(8, (void**)&originalPresent, presentHook);
        kiero::bind(13, (void**)&originalResizeBuffers, resizebuffersHook);
    }

    hooks = std::make_unique<Hooks>();
}
