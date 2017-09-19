// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "Shaders.hpp"
#include "RenderHelper.hpp"

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

class LinearlyTransformedCosines
{
public:
    LinearlyTransformedCosines(std::shared_ptr<RenderHelper> renderHelper);
    ~LinearlyTransformedCosines();

    void init();

    void setCamera(const glm::mat4& viewMatrix);
    void setLights(std::vector<PolygonalLight> lights);
    void render();

private:
    void loadLookupTextures();

    std::shared_ptr<RenderHelper> _renderHelper;

    std::unique_ptr<fw::ShaderProgram> _ltcShader;
    unsigned int _ltcMat, _ltcMag;

    std::vector<PolygonalLight> _lights;
    glm::mat4 _viewMatrix;
};

}
