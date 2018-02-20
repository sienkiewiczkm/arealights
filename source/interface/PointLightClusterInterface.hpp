#pragma once

#include "glm/glm.hpp"
#include "IInterfaceView.hpp"

namespace arealights {

class PointLightClusterInterface: public IInterfaceView
{
public:
    PointLightClusterInterface();
    virtual ~PointLightClusterInterface();

    void render() override;

    glm::ivec2 getClusterSize() { return _clusterSize; }

private:
    bool _oneWay;
    glm::ivec2 _clusterSize;
};

}
