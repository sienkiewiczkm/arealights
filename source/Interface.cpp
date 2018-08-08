#include "Interface.hpp"

#include "glm/gtc/type_ptr.hpp"
#include <imgui.h>
#include "framework/ImGuiExtensions.hpp"

Interface::Interface() :
    _renderTimeMs{0},
    _sceneRadio{0},
    _metalness{1.0f},
    _roughness{0.25f},
    _activeMaterial{0},
    _surfaceType{0},
    _oneWay{true},
    _clusterSize{2,2},
    _position{0, 0.55f, 0},
    _flux{10.0f},
    _size{1.0f, 1.0f},
    _rotation{0},
    _selectedArealightMethod{AREALIGHT_LTC}
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
        renderLightSettings();
        renderSceneSettings();

        if (ImGui::CollapsingHeader("Point Light Cluster")) {
            ImGui::SliderInt2("Cluster size", glm::value_ptr(_clusterSize), 1, 32);
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

void Interface::renderSceneSettings()
{
    if (ImGui::CollapsingHeader("Scene"))
    {
        ImGui::Text("Floor settings");

        ImGui::RadioButton("Uniform", &_sceneRadio, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Textured", &_sceneRadio, 1);

        if (_sceneRadio == 0)
        {
            ImGui::RadioButton("Metal", &_surfaceType, 0); ImGui::SameLine();
            ImGui::RadioButton("Conductor", &_surfaceType, 1); ImGui::SameLine();
            ImGui::RadioButton("Semiconductor", &_surfaceType, 2);

            if (_surfaceType == 0) {
                _metalness = 1.0f;
            } else if (_surfaceType == 1) {
                _metalness = 0.0f;
            } else {
                ImGui::SliderFloat("Metalness", &_metalness, 0.0f, 1.0f);
            }


            ImGui::SliderFloat("Roughness", &_roughness, 0.001f, 1.0f);
        }
        else
        {
            ImGui::ListBox("Material", &_activeMaterial, _materialNames);
        }
    }
}

void Interface::renderLightSettings()
{
    if (!ImGui::CollapsingHeader("Light Settings")) {
        return;
    }

    const char* arealightMethods[] = {
        "Disabled",
        "Point light cluster",
        "Ground Truth (GGX)",
        "Linearly Transformed Cosines"
    };

    ImGui::Combo("Method", reinterpret_cast<int*>(&_selectedArealightMethod), arealightMethods, 4);

    ImGui::DragFloat3("Position", glm::value_ptr(_position), 0.1f);
    ImGui::DragFloat2("Size", glm::value_ptr(_size), 0.1f, 0.01f, 100.0f);
    ImGui::DragFloat2("Rotation", glm::value_ptr(_rotation), 1.0f, -360.0f, 360.0f);
    ImGui::DragFloat("Flux", &_flux, 0.05f, 0.0f, 10000.0f);
}
