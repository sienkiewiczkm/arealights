// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Application.hpp"

namespace arealights
{

Application::Application()
{
}

Application::~Application()
{
}

void Application::onCreate()
{
    ImGuiApplication::onCreate();
}

void Application::onDestroy()
{
    ImGuiApplication::onDestroy();
}

void Application::onUpdate(
    const std::chrono::high_resolution_clock::duration& deltaTime
)
{
    ImGuiApplication::onUpdate(deltaTime);
}

void Application::onRender()
{
    ImGuiApplication::onRender();
}

}
