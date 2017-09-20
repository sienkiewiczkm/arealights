// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "PointLightCluster.hpp"
#include "Logging.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

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

void PointLightCluster::setCamera(
    const glm::mat4& viewMatrix,
    const glm::mat4& projMatrix
)
{
    _viewMatrix = viewMatrix;
    _projMatrix = projMatrix;
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

    _sphereLightVolumeShader = _renderHelper->makeSimpleShader(
        "../assets/SphereLightVolume.vs",
        "../assets/SphereLightVolume.fs"
    );
}

void PointLightCluster::render()
{
    if (_lights.size() == 0)
    {
        return;
    }

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    glDisable(GL_DEPTH_TEST);

    // hack: Hardcoded quad type
    const int numLightsX = 4;
    const int numLightsY = 4;

    const float attConst = 1.0f;
    const float attLinear = 0.7f;
    const float attQuadratic = 1.8f;
    const float Imax = 1.0f;

    float invMinChannel = 256.0f / 6;
    float nominatorSecondTerm = std::sqrtf(
        attLinear*attLinear - 4*attQuadratic*(attConst - Imax * invMinChannel)
    );
    float radius = (-attLinear + nominatorSecondTerm) / (2 * attQuadratic);

    // Prepare stencil
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);

    auto normalWorldMatrix = glm::transpose(glm::inverse(_lights[0].transformation));
    auto normalViewMatrix = glm::transpose(glm::inverse(_viewMatrix)) * normalWorldMatrix;
    glm::vec3 lightViewNormal = normalViewMatrix * glm::vec4{0.0f, 0.0f, 1.0f, 0.0f};

    for (int i = 0; i < numLightsX; ++i)
    for (int j = 0; j < numLightsY; ++j)
    {
        float x = (i/static_cast<float>(numLightsX-1)) - 0.5f;
        float y = (j/static_cast<float>(numLightsY-1)) - 0.5f;

        glm::vec4 pointLightPosition{x, y, 0.0f, 1.0f};
        auto worldLightPosition = _lights[0].transformation * pointLightPosition;
        glm::vec3 lightViewPosition = _viewMatrix * worldLightPosition;

        glm::mat4 scaleMatrix = glm::scale({}, glm::vec3{radius});

        // Mark light influence on stencil buffer
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

        glEnable(GL_DEPTH_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        _sphereLightVolumeShader->use();
        _sphereLightVolumeShader->setUniform("viewMatrix", _viewMatrix);
        _sphereLightVolumeShader->setUniform("projMatrix", _projMatrix);
        _sphereLightVolumeShader->setUniform(
            "worldMatrix",
            glm::translate({}, glm::vec3{worldLightPosition}) * scaleMatrix
        );

        _renderHelper->drawUnitSphere();

        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        // Light area marked by stencil
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        _shader->use();
        _shader->setUniform("TargetTexture", 0);
        _shader->setUniform("NormalTexture", 1);
        _shader->setUniform("PositionTexture", 2);

        _shader->setUniform("NumPointLights", numLightsX * numLightsY);
        _shader->setUniform("LightViewPosition", lightViewPosition);
        _shader->setUniform("LightViewNormal", lightViewNormal);

        _renderHelper->drawFullScreenQuad();
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
}

}
