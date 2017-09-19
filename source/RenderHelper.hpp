// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "OpenGLApplication.hpp"
#include "Shaders.hpp"

namespace arealights
{

class RenderHelper
{
public:
    RenderHelper(fw::OpenGLApplication* glApplication);
    ~RenderHelper();

    std::unique_ptr<fw::ShaderProgram> makeSimpleShader(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath
    );

    void drawFullScreenQuad();
    glm::ivec2 getFramebufferSize() const;

private:
    fw::OpenGLApplication *_glApplication;
    unsigned int _quadVAO, _quadVBO;
};

}
