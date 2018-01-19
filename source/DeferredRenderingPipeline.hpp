// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <memory>
#include "glm/glm.hpp"
#include "framework/Shaders.hpp"

namespace arealights
{

class DeferredRenderingPipeline
{
public:
    DeferredRenderingPipeline();
    virtual ~DeferredRenderingPipeline();

    void create(glm::ivec2 resolution);

    void startGeometryPass();
    void endGeometryPass();

    void setModelMatrix(glm::mat4 modelMatrix);
    void setViewMatrix(glm::mat4 viewMatrix);
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setMaterialID(float materialID);

    virtual void startLightingPass();
    virtual void endLightingPass();

    unsigned int getFramebuffer() { return _gBuffer; }
    glm::ivec2 getFramebufferSize() { return _resolution; }

    unsigned int getPositionBuffer() { return _gPosition; }
    unsigned int getNormalBuffer() { return _gNormal; }
    unsigned int getColorBuffer() { return _gColor; }

    std::shared_ptr<fw::ShaderProgram> getShader()
    {
        return _geometryPassShader;
    }

private:
    glm::ivec2 _resolution;
    unsigned int _gBuffer;
    unsigned int _gPosition, _gNormal, _gColor;

    std::shared_ptr<fw::ShaderProgram> _geometryPassShader;
};

}
