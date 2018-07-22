#include "Interface.hpp"

#include <imgui.h>

Interface::Interface() : _renderTimeMs{0}
{
}

Interface::~Interface()
{
}

void Interface::init()
{
    applyImGuiStyle();
}

void Interface::render()
{
    if (ImGui::Begin("Arealights")) {
        for (auto& view: _views) {
            view->render();
        }
    }

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Arealights (2017) by Kamil Sienkiewicz - Master Thesis (frame render time: %.4fms)", _renderTimeMs);
        ImGui::EndMainMenuBar();
    }

    ImGui::End();
}

void Interface::applyImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 3;
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.08f, 0.08f, 0.16f, 0.71f);

    ImGui::GetIO().FontGlobalScale = 1.4;
}

void Interface::addView(std::shared_ptr<IInterfaceView> view)
{
    _views.push_back(view);
}
