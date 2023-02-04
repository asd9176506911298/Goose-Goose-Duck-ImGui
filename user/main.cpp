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

bool b_enableFog = false;
bool b_enableSpeed = false;
bool b_enableZoom = false;
bool b_enableNoSkillCoolDown = false;
bool b_enableNoClip = false;
bool b_enableESPBox = false;
bool b_enableESPLine = false;

float f_zoomSize = 5;
float f_movementSpeed = 5;

void dLocalPlayer_Update(LocalPlayer* __this, MethodInfo* method)
{
    if (__this)
    {
        if((*LobbySceneHandler__TypeInfo)->static_fields->InGameScene)
        {
            if ((*LocalPlayer__TypeInfo)->static_fields->Instance)
                (*LocalPlayer__TypeInfo)->static_fields->Instance->fields.DLKDHICPDNC->fields.m_AnimatedTarget = 0;

            if (b_enableZoom)
            {
                (*LocalPlayer__TypeInfo)->static_fields->Instance->fields.GBGDLECMLJA->fields.m_Lens.OrthographicSize = f_zoomSize;
                //LocalPlayer_OverrideOrthographicSize(__this, f_zoomSize, method);
            }
            else
            {
                (*LocalPlayer__TypeInfo)->static_fields->Instance->fields.GBGDLECMLJA->fields.m_Lens.OrthographicSize = 5;
                //LocalPlayer_ResetOrthographicSize(__this, 0);
            }

            if (b_enableFog)
            {
                auto roof = (*LobbySceneHandler__TypeInfo)->static_fields->Instance->fields.roofHandler;
                auto blackGameObject = (*LobbySceneHandler__TypeInfo)->static_fields->Instance->fields.blackGameObject;
                auto shader = __this->fields.fogOfWar->fields.shader;
                auto fogofwar = (__this->fields.fogOfWar);

                if (roof)
                    RoofHandler_NAODGMMMIHL(roof, 1, 0);    //Remove Roof

                GameObject_SetActive(blackGameObject, 0, 0);//blackGameObject

                __this->fields.fogOfWar->fields.baseViewDistance = ObscuredFloat_op_Implicit(100, 0);    //ViewDistance
                __this->fields.fogOfWar->fields.viewDistanceMultiplier = ObscuredFloat_op_Implicit(1, 0);//viewDistanceMultiplier

                fogofwar->fields.LFCAGPPBAAH = 1;                 //Stop Calculate Maybe?

                FogOfWarHandler_UpdateFieldOfView(fogofwar, 1, 0);//Update View
                GameObject_SetActive(shader, 0, 0);               //Remove Shader
            }
        
            if (b_enableSpeed)
            {
                (*LocalPlayer__TypeInfo)->static_fields->movementSpeed = ObscuredFloat_op_Implicit(f_movementSpeed, 0);
            }
            else
            {
                (*LocalPlayer__TypeInfo)->static_fields->movementSpeed = ObscuredFloat_op_Implicit(5, 0);
            }   

            if(__this->fields.Player != nullptr)
            {
                if (b_enableNoClip)
                {
                    Behaviour_set_enabled((Behaviour*)__this->fields.Player->fields.playerCollider, 0, 0);
                }
                else
                {
                     Behaviour_set_enabled((Behaviour*)__this->fields.Player->fields.playerCollider, 1, 0);
                }
            }
        }
    }

    LocalPlayer_Update(__this, method);
}

//collider + 0x10 + 0x38 true false

void dUICooldownButton_Update(UICooldownButton* __this, MethodInfo* method)
{
    if(__this)
    {
        if (b_enableNoSkillCoolDown)
        {
            UICooldownButton_set_Cooldown(__this, ObscuredFloat_op_Implicit(0, 0), 0);
        }
    }

    UICooldownButton_Update(__this, method);
}


LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

ImVec2 GetWindowSize()
{
    if (Screen_get_fullScreen(nullptr))
    {
        RECT rect;
        GetWindowRect(window, &rect);

        return { (float)(rect.right - rect.left),  (float)(rect.bottom - rect.top) };
    }

    return { (float)Screen_get_width(nullptr), (float)Screen_get_height(nullptr) };

}

static ImVec2 winSize;
static Camera* mainCamera;

void DrawESP()
{
    if (!(*LobbySceneHandler__TypeInfo)->static_fields->InGameScene)
        return;

    ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
    
    winSize = GetWindowSize();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(winSize);

    auto pDrawList = ImGui::GetWindowDrawList();

    mainCamera = Camera_get_main(0);
    
    auto items = ((*PlayerController__TypeInfo)->static_fields->playersListWithAgoraIDs->fields._entries->vector);
    auto sizes = (*PlayerController__TypeInfo)->static_fields->playersListWithAgoraIDs->fields._count;

    float width;
    float height;

    if(b_enableZoom)
    {
        width = winSize.x / 16.0f / (f_zoomSize / 6.0f);
        height = winSize.y / 10.0f / (f_zoomSize / 6.0f);
    }
    else
    {
        width = winSize.x / 16.0f;
        height = winSize.y / 10.0f;
    }

    if (items != nullptr && sizes > 0)
    {
        auto myPos = (*LocalPlayer__TypeInfo)->static_fields->Instance->fields.Player->fields.FIPBPONCKIE;
        for (int i = 0; i < sizes; i++)
        {
            if (items[i].value && !items[i].value->fields.isLocal)
            {
                auto Pos = items[i].value->fields.FIPBPONCKIE;
                auto Pos2D = Camera_WorldToScreenPoint_1(mainCamera, Pos, 0);
                auto myPos2D = Camera_WorldToScreenPoint_1(mainCamera, myPos, 0);
                Vector2 newPos = Vector2{ Pos2D.x, winSize.y - Pos2D.y };
                Vector2 newmyPos = Vector2{ myPos2D.x, winSize.y - myPos2D.y };
                if (b_enableESPLine)
                    pDrawList->AddLine({ newmyPos.x, newmyPos.y }, { newPos.x, newPos.y }, ImColor(255, 0, 0), 1);
                if (b_enableESPBox)
                    pDrawList->AddRect({ newPos.x - width / 2, newPos.y - height / 2 }, { newPos.x + width / 2, newPos.y + height / 2 }, ImColor(255, 0, 0));
            }
        }
    }

    ImGui::End();
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

    ImGui::Checkbox("RemoveFogOfWar", &b_enableFog);
    ImGui::Checkbox("NoSkillCoolDown", &b_enableNoSkillCoolDown);
    ImGui::Checkbox("NoClip", &b_enableNoClip);
    
    ImGui::Checkbox("SpeedValue", &b_enableSpeed); ImGui::SameLine();
    ImGui::SliderFloat("##SpeedValue", &f_movementSpeed, 5, 15);

    ImGui::Checkbox("Zoom", &b_enableZoom); ImGui::SameLine();
    ImGui::SliderFloat("##zoom value", &f_zoomSize, 0.5, 40);

    ImGui::Checkbox("ESPBox", &b_enableESPBox);
    ImGui::Checkbox("ESPLine", &b_enableESPLine);

    DrawESP();

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
    HOOKFUNC(UICooldownButton_Update);
    
    
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
    UNHOOKFUNC(UICooldownButton_Update);
    

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