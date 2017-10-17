// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "PLCConfigurationUI.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace arealights {

    PLCConfigurationUI::PLCConfigurationUI() :
            _clusterSize{4, 4} {
    }

    PLCConfigurationUI::~PLCConfigurationUI() {
    }

    void PLCConfigurationUI::update() {
        if (ImGui::Begin("Point Light Cluster Configuration")) {
            ImGui::SliderInt2("Cluster size", glm::value_ptr(_clusterSize), 1, 32);
        }

        ImGui::End();
    }

}
