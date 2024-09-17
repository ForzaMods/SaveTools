//
// Created by merika on 9/8/2024.
//

#include "Window.hpp"
#include "Application.hpp"

namespace forza_saveswapper {

    Application::Application(const std::string& windowName) {
        m_WindowName = windowName;
        m_Window = std::make_unique<Window>(*this, m_WindowName);
    }

    Application::~Application() {
        m_Window = nullptr;
    }

    void Application::Run() const {
        m_Window->Initialize();

        while (m_Window->GetIsRunning()) {
            Loop();
        }

        m_Window->Cleanup();
    }

    void Application::Loop() const {
        m_Window->BeginRender();
        m_Window->Render();
        m_Window->EndRender();
    }
} // forza_saveswapper