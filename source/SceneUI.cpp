// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "SceneUI.hpp"
#include "imgui.h"
#include "framework/ImGuiExtensions.hpp"

namespace arealights
{

SceneUI::SceneUI():
    _showUI{true},
    _sceneRadio{0},
    _metalness{1.0f},
    _roughness{0.25f},
    _activeMaterial{0}
{
}

SceneUI::~SceneUI()
{
}

void SceneUI::update()
{
    if (ImGui::Begin("Scene"))
    {
        ImGui::RadioButton("Solid floor", &_sceneRadio, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Textured floor", &_sceneRadio, 1);

        if (_sceneRadio == 0)
        {
            ImGui::SliderFloat("Metalness", &_metalness, 0.0f, 1.0f);
            ImGui::SliderFloat("Roughness", &_roughness, 0.001f, 1.0f);
        }
        else
        {
            ImGui::ListBox("Material", &_activeMaterial, _materialNames);
        }


    }

    ImGui::End();
}

}
