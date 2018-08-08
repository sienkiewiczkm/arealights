// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "framework/OpenGLApplication.hpp"
#include "framework/Shaders.hpp"
#include "framework/Mesh.hpp"
#include "framework/Vertices.hpp"

namespace arealights
{

/***
 * @brief Rendering helpers containing repeatedly used operations in the preview application.
 */
class RenderHelper
{
public:
    RenderHelper(fw::OpenGLApplication* glApplication);
    ~RenderHelper();

    std::unique_ptr<fw::ShaderProgram> makeSimpleShader(
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath
    );

    void drawUnitSphere();
    void drawFullScreenQuad();

    glm::ivec2 getFramebufferSize() const;

private:
    fw::OpenGLApplication *_glApplication;
    unsigned int _quadVAO, _quadVBO;

    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _sphereMesh;
};

}
