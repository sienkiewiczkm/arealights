// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "PointLightCluster.hpp"
#include "Logging.hpp"

namespace arealights
{

PointLightCluster::PointLightCluster(
    std::shared_ptr<RenderHelper> renderHelper
):
    _renderHelper{renderHelper}
{
}

PointLightCluster::~PointLightCluster()
{
}

void PointLightCluster::setCamera(const glm::mat4& viewMatrix)
{
    _viewMatrix = viewMatrix;
}

void PointLightCluster::setLights(std::vector<PolygonalLight> lights)
{
    _lights = lights;
}

void PointLightCluster::init()
{
    _shader = _renderHelper->makeSimpleShader(
        "../assets/PointLightCluster.vs",
        "../assets/PointLightCluster.fs"
    );
}

void PointLightCluster::render()
{
    if (_lights.size() == 0)
    {
        return;
    }

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);

    _shader->use();
    _shader->setUniform("TargetTexture", 0);
    _shader->setUniform("NormalTexture", 1);
    _shader->setUniform("PositionTexture", 2);

    // hack: Hardcoded quad type
    const int numLightsX = 4;
    const int numLightsY = 4;
    _shader->setUniform("NumPointLights", numLightsX * numLightsY);

    for (int i = 0; i < numLightsX; ++i)
    for (int j = 0; j < numLightsY; ++j)
    {
        float x = (i/static_cast<float>(numLightsX-1)) - 0.5f;
        float y = (j/static_cast<float>(numLightsY-1)) - 0.5f;

        glm::vec4 pointLightPosition{x, y, 0.0f, 1.0f};
        auto worldLightPosition = _lights[0].transformation * pointLightPosition;
        glm::vec3 lightViewPosition = _viewMatrix * worldLightPosition;
        _shader->setUniform("LightViewPosition", lightViewPosition);
        _renderHelper->drawFullScreenQuad();
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

}
