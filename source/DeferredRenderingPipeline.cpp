// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "DeferredRenderingPipeline.hpp"
#include "OpenGL.hpp"
#include "Logging.hpp"

namespace arealights
{

DeferredRenderingPipeline::DeferredRenderingPipeline()
{
}

DeferredRenderingPipeline::~DeferredRenderingPipeline()
{
}

void DeferredRenderingPipeline::create(glm::ivec2 resolution)
{
    _resolution = resolution;

    glGenFramebuffers(1, &_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

    glGenTextures(1, &_gPosition);
    glBindTexture(GL_TEXTURE_2D, _gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, resolution.x, resolution.y, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gPosition, 0);

    glGenTextures(1, &_gNormal);
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, resolution.x, resolution.y, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gNormal, 0);

    glGenTextures(1, &_gColor);
    glBindTexture(GL_TEXTURE_2D, _gColor);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, resolution.x, resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gColor, 0);

    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(ERROR) << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    fw::Shader vertexShader;
    vertexShader.addSourceFromFile("../assets/DeferredGeometryPass.vs");
    vertexShader.compile(GL_VERTEX_SHADER);

    fw::Shader fragmentShader;
    fragmentShader.addSourceFromFile("../assets/DeferredGeometryPass.fs");
    fragmentShader.compile(GL_FRAGMENT_SHADER);

    _geometryPassShader = std::make_shared<fw::ShaderProgram>();
    _geometryPassShader->attach(&vertexShader);
    _geometryPassShader->attach(&fragmentShader);
    _geometryPassShader->link();
}

void DeferredRenderingPipeline::startGeometryPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
    glViewport(0, 0, _resolution.x, _resolution.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _geometryPassShader->use();
}

void DeferredRenderingPipeline::endGeometryPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderingPipeline::setModelMatrix(glm::mat4 modelMatrix)
{
    auto location = _geometryPassShader->getUniformLoc("modelMatrix");
    _geometryPassShader->setUniform(location, modelMatrix);
}

void DeferredRenderingPipeline::setViewMatrix(glm::mat4 viewMatrix)
{
    auto location = _geometryPassShader->getUniformLoc("viewMatrix");
    _geometryPassShader->setUniform(location, viewMatrix);
}

void DeferredRenderingPipeline::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    auto location = _geometryPassShader->getUniformLoc("projectionMatrix");
    _geometryPassShader->setUniform(location, projectionMatrix);
}

void DeferredRenderingPipeline::setMaterialID(float materialID)
{
    auto location = _geometryPassShader->getUniformLoc("MaterialID");
    _geometryPassShader->setUniform(location, materialID);
}


void DeferredRenderingPipeline::startLightingPass()
{
}

void DeferredRenderingPipeline::endLightingPass()
{
}

}
