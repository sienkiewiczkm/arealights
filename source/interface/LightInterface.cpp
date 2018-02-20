#include "LightInterface.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "../framework/ImGuiExtensions.hpp"

using namespace arealights;

LightInterface::LightInterface():
    _position{0, 0.55f, 0},
    _flux{10.0f},
    _size{1.0f, 1.0f},
    _rotation{0}
{
}

LightInterface::~LightInterface()
{
}

void LightInterface::render()
{
    if (!ImGui::CollapsingHeader("Light Settings")) {
        return;
    }

    const char* arealightMethods[] = {
        "Point light cluster",
        "Ground Truth (GGX)",
        "Linearly Transformed Cosines"
    };

    ImGui::Combo("Method", reinterpret_cast<int*>(&_selectedArealightMethod), arealightMethods, 3);

    ImGui::DragFloat3("Position", glm::value_ptr(_position), 0.1f);
    ImGui::DragFloat2("Size", glm::value_ptr(_size), 0.1f, 0.01f, 100.0f);
    ImGui::DragFloat2("Rotation", glm::value_ptr(_rotation), 1.0f, -360.0f, 360.0f);
    ImGui::DragFloat("Flux", &_flux, 0.05f, 0.0f, 10000.0f);
}
