// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include "RenderHelper.hpp"
#include "Shaders.hpp"
#include "PolygonalLight.hpp"

namespace arealights
{

class PointLightCluster
{
public:
    PointLightCluster(std::shared_ptr<RenderHelper> renderHelper);
    ~PointLightCluster();

    void setCamera(const glm::mat4& viewMatrix);
    void setLights(std::vector<PolygonalLight> lights);

    void init();
    void render();

private:
    std::shared_ptr<RenderHelper> _renderHelper;
    std::unique_ptr<fw::ShaderProgram> _shader;
    std::vector<PolygonalLight> _lights;
    glm::mat4 _viewMatrix;
};

}

