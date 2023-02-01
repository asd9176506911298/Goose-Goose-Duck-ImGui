// Generated C++ file by Il2CppInspector - http://www.djkaty.com - https://github.com/djkaty
// Custom injected code entry point

#include "pch-il2cpp.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include "il2cpp-appdata.h"
#include "helpers.h"
#include <vector>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"

#include <d3d11.h>
#include <DXGI.h>
#pragma comment(lib, "d3d11.lib")

#include <iostream>
#include <detours.h>

#define HOOKFUNC(n) DetourAttach(&(PVOID&)n, d##n);
#define UNHOOKFUNC(n) DetourDetach(&(PVOID&)n, d##n);

using namespace app;

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* This, UINT SyncInterval, UINT Flags);
Present oPresent;

static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
HWND window;
WNDPROC oWndProc;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Set the name of your log file here
extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";



bool zoom = false;
bool fog = false;
float zoomValue = 5;

bool once = false;

void dLocalPlayer_Update(LocalPlayer* __this, MethodInfo* method)
{
    if (__this)
    {
        if (zoom)
            LocalPlayer_OverrideOrthographicSize(__this, zoomValue, method);

        if (fog)
        {
            auto roof = (*LobbySceneHandler__TypeInfo)->static_fields->Instance->fields.roofHandler;
            auto blackGameObject = (*LobbySceneHandler__TypeInfo)->static_fields->Instance->fields.blackGameObject;
            auto shader = __this->fields.fogOfWar->fields.shader;
            auto fogofwar = (__this->fields.fogOfWar);

            GameObject_SetActive(blackGameObject, 0, 0);//黑物件

            __this->fields.fogOfWar->fields.baseViewDistance = ObscuredFloat_op_Implicit(100, 0);    //視野距離
            __this->fields.fogOfWar->fields.viewDistanceMultiplier = ObscuredFloat_op_Implicit(1, 0);//視野乘

            fogofwar->fields.LFCAGPPBAAH = 1;                 //停止計算? 應該 這個開了光圈就會停留在原地
                                                              //所以我下面用更新戰爭迷霧 不知道有沒有更好的方法

            RoofHandler_NAODGMMMIHL(roof, 1, 0);              //移除屋頂
            FogOfWarHandler_UpdateFieldOfView(fogofwar, 1, 0);//更新戰爭迷霧
            GameObject_SetActive(shader, 0, 0);               //移除陰影

            //fogofwar->fields.flashlightMode = 0;
            //__this->fields.fogOfWar->fields.CNFEONMONCL = 1; //0xAC
            //__this->fields.GBGDLECMLJA->fields.m_Lens.FarClipPlane = 10.7;
            //RoofHandler_MGMMJGICBIL(roof, 1, 0);
            //__this->fields.Player->fields.fogOfWarEnabled = false;
            //__this->fields.fogOfWar->fields.layerMask.m_Mask = 0;
            //__this->fields.fogOfWar->fields.OEDIBJBNENI = ObscuredInt_op_Implicit(0, 0);
            //RoofHandler_BLEBCLBOFPO(roof, 1, 0);
        }
    }

    LocalPlayer_Update(__this, method);
}


LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall dPresent(IDXGISwapChain* This, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {
        if(SUCCEEDED(This->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice)))
        {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
            DXGI_SWAP_CHAIN_DESC sd;
            This->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            This->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
            pBackBuffer->Release();
            ImGui::CreateContext();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui::StyleColorsDark();
            ImGui_ImplWin32_Init(window);
            ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
            init = true;
        }
        else
            return oPresent(This, SyncInterval, Flags);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Begin("Yuki.kaco Test");

    ImGui::Checkbox("RemoveFogOfWar", &fog);
    ImGui::Checkbox("Zoom", &zoom);
    ImGui::SameLine();
    ImGui::SliderFloat("##zoom value", &zoomValue, 0.5, 40);

    ImGui::End();

    ImGui::Render();
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return oPresent(This, SyncInterval, Flags);
}


bool CreateD3D11Device()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = GetForegroundWindow();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    void** vTable = *(void***)g_pSwapChain;
    oPresent = (Present)vTable[8];
    g_pSwapChain->Release();
    g_pd3dDevice->Release();
    return true;
}

// Custom injected code entry point
void Run(HMODULE hModule)
{
    // Initialize thread data - DO NOT REMOVE
    il2cpp_thread_attach(il2cpp_domain_get());

    // If you would like to write to a log file, specify the name above and use il2cppi_log_write()
     //il2cppi_log_write("Startup");
    CreateD3D11Device();
    // If you would like to output to a new console window, use il2cppi_new_console() to open one and redirect stdout
    //il2cppi_new_console();
    //AllocConsole();
    //FILE* f;
    //freopen_s(&f, "CONOUT$", "w", stdout);

    // Place your custom code here
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach((LPVOID*)&oPresent, dPresent);
    HOOKFUNC(LocalPlayer_Update);
    
    
    DetourTransactionCommit();

    while (!GetAsyncKeyState(VK_END))
    {
        //if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        //    test = !test;

        

        Sleep(50);
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach((LPVOID*)&oPresent, dPresent);

    UNHOOKFUNC(LocalPlayer_Update);
    

    DetourTransactionCommit();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
        
    SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    Sleep(200);

    /*fclose(f);
    FreeConsole();*/
    Sleep(100);
    FreeLibraryAndExitThread(hModule, 0);
    
}