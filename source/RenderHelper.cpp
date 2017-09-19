// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "RenderHelper.hpp"
#include "OpenGL.hpp"

namespace arealights
{

RenderHelper::RenderHelper(fw::OpenGLApplication* glApplication):
    _glApplication{glApplication},
    _quadVAO{0},
    _quadVBO{0}
{
}

RenderHelper::~RenderHelper()
{
    if (_quadVAO) glDeleteVertexArrays(1, &_quadVAO);
    if (_quadVBO) glDeleteBuffers(1, &_quadVBO);
    _quadVAO = _quadVBO = 0;
}

std::unique_ptr<fw::ShaderProgram> RenderHelper::makeSimpleShader(
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath
)
{
    fw::Shader vertexShader, fragmentShader;
    auto shaderProgram = std::make_unique<fw::ShaderProgram>();

    vertexShader.addSourceFromFile(vertexShaderPath);
    vertexShader.compile(GL_VERTEX_SHADER);
    shaderProgram->attach(&vertexShader);

    fragmentShader.addSourceFromFile(fragmentShaderPath);
    fragmentShader.compile(GL_FRAGMENT_SHADER);
    shaderProgram->attach(&fragmentShader);

    shaderProgram->link();
    return shaderProgram;
};


void RenderHelper::drawFullScreenQuad()
{
    if (_quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &_quadVAO);
        glGenBuffers(1, &_quadVBO);
        glBindVertexArray(_quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))
        );
    }

    glBindVertexArray(_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

glm::ivec2 RenderHelper::getFramebufferSize() const
{
    return _glApplication->getFramebufferSize();
}

}
