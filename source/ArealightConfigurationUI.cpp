// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "ArealightConfigurationUI.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace arealights
{

ArealightConfigurationUI::ArealightConfigurationUI():
    _rotationX{},
    _rotationY{},
    _position{0.0f, 0.5f, 0.0f},
    _size{1.0f, 1.0f},
    _selectedArealightMethod{1}
{
}

ArealightConfigurationUI::~ArealightConfigurationUI()
{
}

void ArealightConfigurationUI::update()
{
    if (ImGui::Begin("Arealight"))
    {
        const char* arealightMethods[] = {
            "Debug view",
            "Linearly Transformed Cosines",
            "Point light cluster"
        };

        ImGui::Combo("Method", &_selectedArealightMethod, arealightMethods, 3);

        const char* arealightTypes[] = { "Quad" };
        int selectedItem = 0;
        ImGui::Combo("Shape", &selectedItem, arealightTypes, 1);
        ImGui::DragFloat3("Position", glm::value_ptr(_position), 0.1f);
        ImGui::DragFloat2("Size", glm::value_ptr(_size), 0.1f, 0.01f, 100.0f);
        ImGui::DragFloat("Rotation X", &_rotationX, 1.0f, -360.0f, 360.0f);
        ImGui::DragFloat("Rotation Y", &_rotationY, 1.0f, -360.0f, 360.0f);
        ImGui::End();
    }
}

}
