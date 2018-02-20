#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "IInterfaceView.hpp"

namespace arealights {

enum EArealightMethod {
    AREALIGHT_DISABLED = 0,
    AREALIGHT_CLUSTER = 1,
    AREALIGHT_GROUNDTRUTH = 2,
    AREALIGHT_LTC = 3
};

class LightInterface: public IInterfaceView {
public:
    LightInterface();
    virtual ~LightInterface();

    void render() override;

    const glm::vec3 getPosition() { return _position; }
    const glm::vec2 getSize() { return _size; }
    const glm::vec2 getRotation() { return _rotation; }
    const float getFlux() { return _flux; }

    const EArealightMethod getArealightMethod() { return (EArealightMethod)((int)_selectedArealightMethod + 1); };

private:
    std::vector<int> _arealightMethods;
    EArealightMethod _selectedArealightMethod;

    glm::vec3 _position;
    glm::vec2 _size;
    glm::vec2 _rotation;
    float _flux;
};

}