// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "glm/glm.hpp"

namespace arealights {

    class PLCConfigurationUI {
    public:
        PLCConfigurationUI();
        ~PLCConfigurationUI();

        void update();
        glm::ivec2 getClusterSize() const { return _clusterSize; }

    private:
        glm::ivec2 _clusterSize;
    };

}
