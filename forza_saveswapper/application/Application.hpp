//
// Created by merika on 9/8/2024.
//

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <string>

namespace forza_saveswapper {

class Window;

class Application {
public:
    explicit Application(const std::string& windowName);
    ~Application();

    void Run() const;

private:
    void Loop() const;

private:
    std::string m_WindowName;
    std::unique_ptr<Window> m_Window;
};

} // forza_saveswapper

#endif //APPLICATION_HPP
