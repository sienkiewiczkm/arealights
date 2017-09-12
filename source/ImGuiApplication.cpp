// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "ImGuiApplication.hpp"

#include "imgui.h"
#include "ImGuizmo.h"
#include "ImGuiBinding.hpp"

namespace fw
{

ImGuiApplication::ImGuiApplication()
{
}

ImGuiApplication::~ImGuiApplication()
{
}

void ImGuiApplication::onCreate()
{
    OpenGLApplication::onCreate();
    ImGuiBinding::initialize(_window, false);
}

void ImGuiApplication::onDestroy()
{
    ImGuiBinding::shutdown();
    OpenGLApplication::onDestroy();
}

void ImGuiApplication::onUpdate(
    const std::chrono::high_resolution_clock::duration& deltaTime
)
{
    OpenGLApplication::onUpdate(deltaTime);
    ImGuiBinding::newFrame();
    ImGuizmo::BeginFrame();
}

void ImGuiApplication::onRender()
{
    OpenGLApplication::onRender();
    ImGui::Render();
}

bool ImGuiApplication::onMouseButton(int button, int action, int mods)
{
    ImGuiBinding::mouseButtonCallback(_window, button, action, mods);
    ImGuiIO &io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool ImGuiApplication::onScroll(double xoffset, double yoffset)
{
    ImGuiBinding::scrollCallback(_window, xoffset, yoffset);
    ImGuiIO &io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool ImGuiApplication::onKey(int key, int scancode, int action, int mods)
{
    ImGuiBinding::keyCallback(_window, key, scancode, action, mods);
    ImGuiIO &io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

bool ImGuiApplication::onChar(unsigned int c)
{
    ImGuiBinding::charCallback(_window, c);
    ImGuiIO &io = ImGui::GetIO();
    return io.WantTextInput;
}

}
