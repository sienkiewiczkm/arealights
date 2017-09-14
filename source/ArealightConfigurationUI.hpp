// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "glm/glm.hpp"

namespace arealights
{

class ArealightConfigurationUI
{
public:
    ArealightConfigurationUI();
    ~ArealightConfigurationUI();

    void update();

    glm::vec3 getPosition() const { return _position; }
    glm::vec2 getSize() const { return _size; }
    float getRotationX() const { return _rotationX; }
    float getRotationY() const { return _rotationY; }

private:
    float _rotationX, _rotationY;
    glm::vec3 _position;
    glm::vec2 _size;
};

}
