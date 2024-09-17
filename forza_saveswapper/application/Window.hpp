//
// Created by merika on 8/29/2024.
//

#ifndef GUI_HPP
#define GUI_HPP

#include "Application.hpp"
#include "../fileUtilities/TFileOpenResult.hpp"

#include <memory>
#include <d3d11.h>

namespace forza_saveswapper {

class Window
{
public:
    static constexpr int WIDTH = 320;
    static constexpr int HEIGHT = 170;

    explicit Window(Application& application, std::string  name);

    bool Initialize();
    void Cleanup();

    void BeginRender() noexcept;
    void EndRender() const noexcept;
    void Render() noexcept;
    void ResetDevice() const noexcept;

    [[nodiscard]] IDXGISwapChain* GetSwapchain() const {
        return m_SwapChain;
    }

    [[nodiscard]] POINTS GetPosition() const {
        return m_Position;
    }

    void SetPosition(const POINTS newValue) {
        m_Position = newValue;
    }

    [[nodiscard]] HWND GetWindow() const {
        return m_Window;
    }

    [[nodiscard]] bool GetIsRunning() const {
        return m_IsRunning;
    }

private:
    void CreateDevice() noexcept;
    void DestroyDevice() const noexcept;
    void CreateImGui() const noexcept;

    static void DestroyImGui() noexcept;

    void OpenSave();
    void SwapSave(uint64_t newXuid) const;

private:
    HWND m_Window;
    WNDCLASSEX m_WindowClass{};
    POINTS m_Position{};

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGISwapChain* m_SwapChain;

    Application& m_Application;
    std::string m_Name;
    bool m_IsRunning;

    TFileOpenResult m_FileOpenReuslt;
};

} // forza_saveswapper

#endif //GUI_HPP
