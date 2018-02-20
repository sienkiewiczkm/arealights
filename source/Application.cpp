// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Application.hpp"
#include "framework/BasicMeshes.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "point_light_cluster/PointLightCluster.hpp"
#include "framework/Common.hpp"

namespace arealights
{

Application::Application():
    _restartIncrementalRendering{false},
    _activeMaterial{0}
{
}

Application::~Application()
{
}

void Application::onCreate()
{
    ImGuiApplication::onCreate();

    preloadShaderInclude("../assets/shaderlib/gbuffer.glsl", "/gbuffer.glsl");
    preloadShaderInclude("../assets/shaderlib/cookTorrance.glsl", "/cookTorrance.glsl");

    _renderHelper = std::make_shared<RenderHelper>(this);

    _pointLightCluster = std::make_shared<PointLightCluster>(_renderHelper);
    _pointLightCluster->init();

    _groundTruth = std::make_shared<GroundTruth>(_renderHelper);
    _groundTruth->init();

    _ltc = std::make_shared<LinearlyTransformedCosines>(_renderHelper);
    _ltc->init();

    auto planeMesh = fw::PlaneGenerator::generate({10.0f, 10.0f});
    _planeMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        planeMesh.vertices, planeMesh.indices
    );

    auto arealightMesh = fw::PlaneGenerator::generate({1.0f, 1.0f});
    _arealightMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        arealightMesh.vertices, arealightMesh.indices
    );

    _shaderProgram = _renderHelper->makeSimpleShader(
        "../assets/DebugShader.vs.glsl",
        "../assets/DebugShader.fs.glsl"
    );

    _blitSRGBProgram = _renderHelper->makeSimpleShader(
        "../assets/TextureBlitToSRGB.vs.glsl",
        "../assets/TextureBlitToSRGB.fs.glsl"
    );

    _clusteringShader = _renderHelper->makeSimpleShader(
        "../assets/Clustering.vs.glsl",
        "../assets/Clustering.fs.glsl"
    );

    _camera = std::make_shared<fw::FreeCamera>();
    _camera->setWorldPosition({0.0f, 1.0f, 5.0f});

    _keyboardInput = std::make_shared<fw::GenericKeyboardInput>();
    _mouseInput = std::make_shared<fw::GenericMouseInput>();

    _cameraInputMapper.setCamera(_camera);
    _cameraInputMapper.setKeyboardInput(_keyboardInput);
    _cameraInputMapper.setMouseInput(_mouseInput);

    glm::ivec2 resolution{4*800, 4*600};
    _deferredPipeline = std::make_unique<DeferredRenderingPipeline>();
    _deferredPipeline->create(resolution);

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    LOG(INFO) << "Max texture units: " << maxTextureUnits;

    _scuffedIronMaterial = std::make_shared<Material>();
    _scuffedIronMaterial->setAlbedoTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_basecolor.png");
    _scuffedIronMaterial->setNormalTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_normal.png");
    _scuffedIronMaterial->setMetalnessTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_metallic.png");
    _scuffedIronMaterial->setRoughnessTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_roughness.png");

    _woodPlanksMaterial = std::make_shared<Material>();
    _woodPlanksMaterial->setAlbedoTexture("../assets/textures/WoodPlankFlooring/sculptedfloorboards4_basecolor.png");
    _woodPlanksMaterial->setNormalTexture("../assets/textures/WoodPlankFlooring/sculptedfloorboards4_normal.png");
    _woodPlanksMaterial->setRoughnessTexture("../assets/textures/WoodPlankFlooring/sculptedfloorboards4_roughness.png");
    _woodPlanksMaterial->setMetalnessTexture("../assets/textures/WoodPlankFlooring/sculptedfloorboards4_metalness.png");

    _copperRockMaterial = std::make_shared<Material>();
    _copperRockMaterial->setAlbedoTexture("../assets/textures/copper-rock/copper-rock1-alb.png");
    _copperRockMaterial->setNormalTexture("../assets/textures/copper-rock/copper-rock1-normal.png");
    _copperRockMaterial->setMetalnessTexture("../assets/textures/copper-rock/copper-rock1-metal.png");
    _copperRockMaterial->setRoughnessTexture("../assets/textures/copper-rock/copper-rock1-rough.png");

    _materialMap.push_back({"Scuffed Iron", _scuffedIronMaterial});
    _materialMap.push_back({"Wooden Planks", _woodPlanksMaterial});
    _materialMap.push_back({"Copper infused rock", _copperRockMaterial});

    {
        glGenFramebuffers(1, &_intermediateFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);

        glGenTextures(1, &_intermediateFBOTexture);
        glBindTexture(GL_TEXTURE_2D, _intermediateFBOTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _intermediateFBOTexture, 0);

        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolution.x, resolution.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOG(ERROR) << "Framebuffer not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    // Interface

    _interface.init();

    _lightInterface = std::make_shared<LightInterface>();
    _interface.addView(_lightInterface);

    _sceneInterface = std::make_shared<SceneInterface>();
    _interface.addView(_sceneInterface);

    _clusterInterface = std::make_shared<PointLightClusterInterface>();
    _interface.addView(_clusterInterface);
}

void Application::preloadShaderInclude(const char *filepath, std::string glslIncludePath) const
{
    auto cookTorranceShader = fw::loadASCIITextFile(filepath);

    glNamedStringARB(
        GL_SHADER_INCLUDE_ARB,
        glslIncludePath.size(),
        glslIncludePath.c_str(),
        cookTorranceShader.size(),
        cookTorranceShader.c_str()
    );

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        LOG(ERROR) << err;
    }
}

void Application::onDestroy()
{
    ImGuiApplication::onDestroy();
}

void Application::onUpdate(
    const std::chrono::high_resolution_clock::duration& deltaTime
)
{
    ImGuiApplication::onUpdate(deltaTime);

    _cameraInputMapper.update(deltaTime);
    _restartIncrementalRendering = _cameraInputMapper.hadMovement();

    _interface.render();

    _restartIncrementalRendering = _cameraInputMapper.hadMovement();

    std::vector<std::string> materialNames;
    std::transform(std::begin(_materialMap), std::end(_materialMap), std::back_inserter(materialNames),
        [](const std::pair<std::string, std::shared_ptr<Material>> &p) { return p.first; });

    _sceneInterface->setMaterials(materialNames);
}

void Application::onRender()
{
    auto framebufferSize = getFramebufferSize();
    auto aspectRatio = static_cast<float>(framebufferSize.x) / framebufferSize.y;

    auto viewMatrix = _camera->getViewMatrix();
    auto projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    _deferredPipeline->startGeometryPass();

    glEnable(GL_DEPTH_TEST);

    _deferredPipeline->setModelMatrix({});
    _deferredPipeline->setViewMatrix(viewMatrix);
    _deferredPipeline->setProjectionMatrix(projMatrix);
    _deferredPipeline->setMaterialID(0.1f);

    _materialMap[_sceneInterface->getMaterialId()].second->bind();

    _deferredPipeline->getShader()->setUniform("SolidMode", _sceneInterface->getSceneId());
    _deferredPipeline->getShader()->setUniform("MetalnessConst", _sceneInterface->getMetalness());
    _deferredPipeline->getShader()->setUniform("RoughnessConst", _sceneInterface->getRoughness());

    _planeMesh->render();

    auto lightWorldMatrix = glm::translate({}, _lightInterface->getPosition()) *
        glm::rotate(
            glm::mat4{},
            glm::radians(_lightInterface->getRotation().x),
            glm::vec3{1.0f, 0.0f, 0.0f}
        ) *
        glm::rotate(
            glm::mat4{},
            glm::radians(_lightInterface->getRotation().y),
            glm::vec3{0.0f, 1.0f, 0.0f}
        ) *
        glm::scale({}, glm::vec3{_lightInterface->getSize(), 1.0f});

    auto planeUpMatrix = glm::rotate(glm::mat4{}, glm::radians(90.0f), glm::vec3{1.0f, 0.0f, 0.0f});
    _deferredPipeline->setModelMatrix(lightWorldMatrix * planeUpMatrix);
    _deferredPipeline->setViewMatrix(viewMatrix);
    _deferredPipeline->setProjectionMatrix(projMatrix);
    _deferredPipeline->setMaterialID(0.3f);
    _arealightMesh->render();

    _deferredPipeline->endGeometryPass();

    // this will be done once valid architecture shape will emerge
    _deferredPipeline->startLightingPass();
    _deferredPipeline->endLightingPass();
    auto gbufferResolution = _deferredPipeline->getFramebufferSize();

    auto mode = _lightInterface->getArealightMethod();

    glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);
    glViewport(0, 0, gbufferResolution.x, gbufferResolution.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLbitfield clearMask = GL_DEPTH_BUFFER_BIT;

    const auto isIncremental = (mode == AREALIGHT_GROUNDTRUTH);

    if (mode == AREALIGHT_GROUNDTRUTH && _restartIncrementalRendering) {
        _groundTruth->restartIncrementalRendering();
    }

    if (!isIncremental || _restartIncrementalRendering)
    {
        clearMask |= GL_COLOR_BUFFER_BIT;
        _restartIncrementalRendering = false;
    }

    glClear(clearMask);

    auto positionBuffer = _deferredPipeline->getPositionBuffer();
    auto normalBuffer = _deferredPipeline->getNormalBuffer();
    auto colorBuffer = _deferredPipeline->getColorBuffer();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _deferredPipeline->getFramebuffer());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediateFBO);

    glBlitFramebuffer(
        0, 0, gbufferResolution.x, gbufferResolution.y,
        0, 0, gbufferResolution.x, gbufferResolution.y,
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST
    );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalBuffer);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, positionBuffer);

    if (mode == AREALIGHT_DISABLED)
    {
        renderClusters();
    }
    else if (mode == AREALIGHT_LTC)
    {
        _ltc->setCamera(viewMatrix);
        _ltc->setLights({{{1.0f, 1.0f, 1.0f}, lightWorldMatrix}});
        _ltc->render();
    }
    else if (mode == AREALIGHT_CLUSTER)
    {
        _pointLightCluster->setLightFlux(_lightInterface->getFlux());
        _pointLightCluster->setClusterSize(_clusterInterface->getClusterSize());
        _pointLightCluster->setCamera(viewMatrix, projMatrix);
        _pointLightCluster->setLights({{{1.0f, 1.0f, 1.0f}, lightWorldMatrix}});
        _pointLightCluster->render();
    }
    else if (mode == AREALIGHT_GROUNDTRUTH)
    {
        _groundTruth->setCamera(viewMatrix, projMatrix);
        _groundTruth->setLights({{{1.0f, 1.0f, 1.0f}, lightWorldMatrix}});
        _groundTruth->render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClearColor(0.007f, 0.11f, 0.15f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    _blitSRGBProgram->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _intermediateFBOTexture);
    _blitSRGBProgram->setUniform("TargetTexture", 0);

    _renderHelper->drawFullScreenQuad();

    ImGuiApplication::onRender();

    // buffers are swapped after onRender by default

    _keyboardInput->nextFrame();
    _mouseInput->nextFrame();
}

bool Application::onMouseButton(int button, int action, int mods)
{
    if (ImGuiApplication::onMouseButton(button, action, mods)) { return true; }

    if (action == GLFW_PRESS)
    {
        _mouseInput->buttonDown(button);
    }
    else if (action == GLFW_RELEASE)
    {
        _mouseInput->buttonUp(button);
    }

    return true;
}

bool Application::onMouseMove(glm::dvec2 newPosition)
{
    if (ImGuiApplication::onMouseMove(newPosition)) { return true; }
    _mouseInput->move(newPosition);
    return true;
}

bool Application::onKey(int key, int scancode, int action, int mods)
{
    if (fw::ImGuiApplication::onKey(key, scancode, action, mods))
    {
        return true;
    }

    if (action == GLFW_PRESS)
    {
        _keyboardInput->keyDown(key);
    }
    else if (action == GLFW_RELEASE)
    {
        _keyboardInput->keyUp(key);
    }

    return true;
}

void Application::renderClusters()
{
    _clusteringShader->use();

    _clusteringShader->setUniform("GBufferC", 0);
    _clusteringShader->setUniform("GBufferB", 1);
    _clusteringShader->setUniform("GBufferA", 2);

    _renderHelper->drawFullScreenQuad();
}

}
