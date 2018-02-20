#include "PointLightClusterInterface.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

using namespace arealights;

PointLightClusterInterface::PointLightClusterInterface():
    _oneWay{true},
    _clusterSize{2,2}
{
}

PointLightClusterInterface::~PointLightClusterInterface()
{
}

void PointLightClusterInterface::render()
{
    if (ImGui::CollapsingHeader("Point Light Cluster")) {
        ImGui::Checkbox("One way?", &_oneWay);
        ImGui::SliderInt2("Cluster size", glm::value_ptr(_clusterSize), 1, 32);
    }
}

