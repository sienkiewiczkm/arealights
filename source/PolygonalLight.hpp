// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <vector>
#include "glm/glm.hpp"

namespace arealights
{

struct PolygonalLight
{
    PolygonalLight() {}
    PolygonalLight(glm::vec3 color, glm::mat4 transformation):
        color{color}, transformation{transformation} {}

    std::vector<glm::vec3> vertices;
    glm::vec3 color;
    glm::mat4 transformation;
};

}
