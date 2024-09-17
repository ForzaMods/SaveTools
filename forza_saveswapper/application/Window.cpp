//
// Created by merika on 8/29/2024.
//

#include "Window.hpp"
#include "../fileUtilities/FileDialogs.hpp"
#include "../swapper./CBinaryStream.hpp"
#include "../encryption/streams/CFh3Stream.hpp"

#include <imgui_internal.h>
#include <iostream>
#include <thread>
#include <utility>
#include <filesystem>
#include <fstream>

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <winuser.h>

#include "../swapper/CCompileTimeHash.hpp"
#include "../swapper/CRuntimeHash.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter
);

namespace forza_saveswapper {

    LRESULT CALLBACK WindowProcess(
        const HWND window,
        const UINT message,
        const WPARAM wideParameter,
        LPARAM longParameter)
    {
        if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter)) {
            return true;
        }

        static Window* guiInstance = nullptr;
        if (!guiInstance) {
            guiInstance = reinterpret_cast<Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
        }

        switch (message) {
            case WM_SIZE: {
                if (guiInstance && guiInstance->GetSwapchain() && wideParameter != SIZE_MINIMIZED) {
                    guiInstance->ResetDevice();
                }
                return 0;
            }

            case WM_SYSCOMMAND: {
                if ((wideParameter & 0xfff0) == SC_KEYMENU) {
                    return 0;
                }
            }

            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            }

            case WM_LBUTTONDOWN: {
                if (guiInstance) {
                    guiInstance->SetPosition(MAKEPOINTS(longParameter));
                }
                return 0;
            }

            case WM_MOUSEMOVE: {
                if (guiInstance && wideParameter == MK_LBUTTON) {
                    const auto [x, y] = MAKEPOINTS(longParameter);
                    auto rect = RECT{ };

                    const auto guiWindow = guiInstance->GetWindow();
                    GetWindowRect(guiWindow, &rect);

                    const auto [windowX, windowY] = guiInstance->GetPosition();
                    rect.left += x - windowX;
                    rect.top += y - windowY;

                    POINT cursorPos;
                    if (!GetCursorPos(&cursorPos)) {
                        return 0;
                    }

                    if (cursorPos.y - rect.top < 25) {
                        SetWindowPos(
                            guiWindow,
                            HWND_TOPMOST,
                            rect.left,
                            rect.top,
                            0, 0,
                            SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                        );
                    }
                }

                return 0;
            }
            default:
                return DefWindowProc(window, message, wideParameter, longParameter);
        }
    }

    Window::Window(Application& application, std::string name)
        : m_Window(nullptr),
          m_Device(nullptr),
          m_DeviceContext(nullptr),
          m_SwapChain(nullptr),
          m_Application(application),
          m_Name (std::move(name)),
          m_IsRunning(true) {
        m_FileOpenReuslt.SelectedFileName = "No save file picked!";
    }

    bool Window::Initialize() {
        m_WindowClass.cbSize = sizeof(WNDCLASSEX);
        m_WindowClass.style = CS_CLASSDC;
        m_WindowClass.lpfnWndProc = WindowProcess;
        m_WindowClass.cbClsExtra = 0;
        m_WindowClass.cbWndExtra = sizeof(Window*);
        m_WindowClass.hInstance = GetModuleHandleA(nullptr);
        m_WindowClass.hIcon = nullptr;
        m_WindowClass.hCursor = nullptr;
        m_WindowClass.hbrBackground = nullptr;
        m_WindowClass.lpszMenuName = nullptr;
        m_WindowClass.lpszClassName = "class001";
        m_WindowClass.hIconSm = nullptr;

        RegisterClassEx(&m_WindowClass);
        m_Window = CreateWindowEx(
            0,
            "class001",
            m_Name.c_str(),
            WS_POPUP,
            100,
            100,
            WIDTH,
            HEIGHT,
            nullptr,
            nullptr,
            m_WindowClass.hInstance,
            nullptr
        );

        if (!m_Window) {
            return false;
        }

        SetWindowLongPtr(m_Window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        ShowWindow(m_Window, SW_SHOWDEFAULT);
        UpdateWindow(m_Window);
        CreateDevice();
        CreateImGui();
        return true;
    }

    void Window::Cleanup() {
        DestroyImGui();
        DestroyDevice();
        if (m_Window) {
            DestroyWindow(m_Window);
            UnregisterClass(m_WindowClass.lpszClassName, m_WindowClass.hInstance);
            m_Window = nullptr;
        }
    }

    void Window::CreateDevice() noexcept {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = WIDTH;
        swapChainDesc.BufferDesc.Height = HEIGHT;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.OutputWindow = m_Window;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.Flags = 0;

        constexpr D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        D3D_FEATURE_LEVEL featureLevel;

        if (const HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            featureLevels,
            std::size(featureLevels),
            D3D11_SDK_VERSION,
            &swapChainDesc,
            &m_SwapChain,
            &m_Device,
            &featureLevel,
            &m_DeviceContext
        ); FAILED(hr)) {
            IM_ASSERT(0);
        }
    }

    void Window::ResetDevice() const noexcept {
        ImGui_ImplDX11_InvalidateDeviceObjects();

        if (const HRESULT hr = m_SwapChain->ResizeBuffers(
            0,
            0,
            0,
            DXGI_FORMAT_UNKNOWN,
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        ); FAILED(hr)) {
            IM_ASSERT(0);
        }

        ImGui_ImplDX11_CreateDeviceObjects();
    }

    void Window::DestroyDevice() const noexcept {
        if (m_DeviceContext) {
            m_DeviceContext->Release();
        }
        if (m_Device) {
            m_Device->Release();
        }
        if (m_SwapChain) {
            m_SwapChain->Release();
        }
    }

    void Window::CreateImGui() const noexcept {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::Spectrum::LoadFont(19);

        ImGui_ImplWin32_Init(m_Window);
        ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
        ImGuiStyle* style = &ImGui::GetStyle();
        style->GrabRounding = 4.0f;
        style->WindowRounding = 0.f;

        ImVec4* colors = style->Colors;
        colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::Static::WHITE);
        colors[ImGuiCol_TextDisabled] = ImGui::ColorConvertU32ToFloat4(ImGui::Spectrum::Static::GRAY700);
    }

    void Window::DestroyImGui() noexcept {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void Window::OpenSave() {
        m_FileOpenReuslt = FileDialogs::OpenFile();
        if (!m_FileOpenReuslt.Succeeded) {
            MessageBoxA(nullptr, "The save file opening failed!", "Error", MB_ICONERROR);
        }
    }

    void Window::SwapSave(const uint64_t newXuid) const {
        if (!m_FileOpenReuslt.Succeeded) {
            MessageBoxA(nullptr, "The file open must succeed first!", "Error", MB_ICONERROR);
            return;
        }

        const std::filesystem::path inputPath(m_FileOpenReuslt.Path);
        if (!exists(inputPath)) {
            MessageBoxA(nullptr, "The input save doesn't exist anymore!", "Error", MB_ICONERROR);
            return;
        }

        std::ifstream input(inputPath.string().c_str(), std::ios_base::binary);
        const auto input_size = static_cast<uint32_t>(file_size(inputPath));
        const auto encryptedFile = std::make_unique<CFh3DecryptionStream>(input, input_size);
        if (!*encryptedFile) {
            MessageBoxA(nullptr, "Couldn't decrypt the file!", "Error", MB_ICONERROR);
            return;
        }

        const auto output = std::make_shared<std::stringstream>();
        encryptedFile->ReadData(*output);

        CBinaryStream stream(output);
        const auto profileOptionsHeaderHash = stream.Read<uint32_t>();
        if (profileOptionsHeaderHash != CCompileTimeHash("profile").value()) {
            MessageBoxA(nullptr, "Profile options header hash didn't match!", "Error", MB_ICONERROR);
            return;
        }

        const auto profileOptionsSize = stream.Read<uint32_t>();
        stream.SetPosition(stream.GetPosition() + static_cast<std::streampos>(profileOptionsSize));

        const auto savestateHeaderHash = stream.Read<uint32_t>();
        if (savestateHeaderHash != CCompileTimeHash("savestate").value()) {
            MessageBoxA(nullptr, "Savestate header hash didn't match!", "Error", MB_ICONERROR);
            return;
        }

        const auto savestateSize = stream.Read<uint32_t>();
        stream.SetPosition(stream.GetPosition() + static_cast<std::streampos>(savestateSize));

        const auto binaryHeaderHash = stream.Read<uint32_t>();
        if (binaryHeaderHash != CCompileTimeHash("binary").value()) {
            MessageBoxA(nullptr, "Binary header hash didn't match!", "Error", MB_ICONERROR);
            return;
        }

        stream.SetPosition(stream.GetPosition() + static_cast<std::streampos>(sizeof(int32_t) * 2));
        stream.ClearStreamFlags();
        stream.Write<uint64_t>(newXuid);
        output->flush();
        output->seekg(0);
        output->seekp(0);

        const auto output2 = std::make_shared<std::ostringstream>();
        const auto es = std::make_unique<CFh3EncryptionStream>(*output2, input_size, std::array<uint8_t, 16>{ }, s_Contexts[0]);
        es->WriteData(*output);

        std::ofstream file(m_FileOpenReuslt.SelectedFileName, std::ios::binary);
        if (!file) {
            MessageBoxA(nullptr, "Failed to open output file!", "Error", MB_ICONERROR);
            return;
        }

        std::string contents = output2->str();
        file.write(contents.c_str(), contents.size());

        if (file.fail()) {
            MessageBoxA(nullptr, "Failed to write to output file!", "Error", MB_ICONERROR);
            return;
        }

        file.close();
        MessageBoxA(nullptr, "Success!", "Information", MB_OK);
    }

    void Window::BeginRender() noexcept {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);

            if (message.message == WM_QUIT) {
                m_IsRunning = !m_IsRunning;
                return;
            }
        }

        while (m_SwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void Window::EndRender() const noexcept {
        ImGui::EndFrame();

        ID3D11RenderTargetView* renderTargetView = nullptr;
        ID3D11Texture2D* backBuffer = nullptr;

        if (SUCCEEDED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer)))) {
            SUCCEEDED(m_Device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView));
            backBuffer->Release();
        }

        m_DeviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);

        constexpr float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        m_DeviceContext->ClearRenderTargetView(renderTargetView, color);

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        renderTargetView->Release();

        if (const HRESULT result = m_SwapChain->Present(1, 0);
            result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET) {
            ResetDevice();
        }
    }

    void Window::Render() noexcept {
        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
        ImGui::Begin(
            m_Name.c_str(),
            &m_IsRunning,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse
        );

        const auto buttonWidth = ImVec2(ImGui::GetContentRegionAvail().x, 0);

        ImGui::Text("Open save: ");
        if (ImGui::Button(m_FileOpenReuslt.SelectedFileName.c_str(), buttonWidth)) {
            OpenSave();
        }

        static uint64_t xuid = 0;

        ImGui::Text("Enter XUID");
        ImGui::InputScalar("##xuid", ImGuiDataType_U64, &xuid, nullptr, nullptr, "%016llX");
        ImGui::SameLine();
        if (ImGui::Button("Grab XUID", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
            ShellExecute(
                nullptr,
                nullptr,
                "https://cxkes.me/xbox/xuid",
                nullptr,
                nullptr,
                SW_SHOW
            );
        }

        if (ImGui::Button("Swap!", buttonWidth)) {
            SwapSave(xuid);
        }
        ImGui::End();
    }

} // forza_saveswapper