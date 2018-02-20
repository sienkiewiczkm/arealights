// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "SceneInterface.hpp"
#include "imgui.h"
#include "../framework/ImGuiExtensions.hpp"

namespace arealights
{

SceneInterface::SceneInterface():
    _sceneRadio{0},
    _metalness{1.0f},
    _roughness{0.25f},
    _activeMaterial{0},
    _surfaceType{0}
{
}

SceneInterface::~SceneInterface()
{
}

void SceneInterface::render()
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

}
