// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Application.hpp"
#include "BasicMeshes.cpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

namespace arealights
{

Application::Application()
{
}

Application::~Application()
{
}

void Application::onCreate()
{
    ImGuiApplication::onCreate();

    _renderHelper = std::make_shared<RenderHelper>(this);

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
        "../assets/DebugShader.vs",
        "../assets/DebugShader.fs"
    );

    _textureBlitShader = _renderHelper->makeSimpleShader(
        "../assets/TextureBlit.vs",
        "../assets/TextureBlit.fs"
    );

    _clusteringShader = _renderHelper->makeSimpleShader(
        "../assets/Clustering.vs",
        "../assets/Clustering.fs"
    );

    _camera = std::make_shared<fw::FreeCamera>();
    _camera->setWorldPosition({0.0f, 1.0f, 5.0f});

    _keyboardInput = std::make_shared<fw::GenericKeyboardInput>();
    _mouseInput = std::make_shared<fw::GenericMouseInput>();

    _cameraInputMapper.setCamera(_camera);
    _cameraInputMapper.setKeyboardInput(_keyboardInput);
    _cameraInputMapper.setMouseInput(_mouseInput);

    _deferredPipeline = std::make_unique<DeferredRenderingPipeline>();
    _deferredPipeline->create({1600, 1200});

    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    LOG(INFO) << "Max texture units: " << maxTextureUnits;
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
    _configurationUI.update();
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
    _planeMesh->render();

    auto lightWorldMatrix = glm::translate({}, _configurationUI.getPosition()) *
        glm::rotate(
            glm::mat4{},
            glm::radians(_configurationUI.getRotationX()),
            glm::vec3{1.0f, 0.0f, 0.0f}
        ) *
        glm::rotate(
            glm::mat4{},
            glm::radians(_configurationUI.getRotationY()),
            glm::vec3{0.0f, 1.0f, 0.0f}
        ) *
        glm::scale({}, glm::vec3{_configurationUI.getSize(), 1.0f});

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

    auto mode = _configurationUI.getArealightMethod();

    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClearColor(0.007f, 0.11f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto positionBuffer = _deferredPipeline->getPositionBuffer();
    auto normalBuffer = _deferredPipeline->getNormalBuffer();
    auto colorBuffer = _deferredPipeline->getColorBuffer();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalBuffer);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, positionBuffer);

    if (mode == 0)
    {
        renderClusters();
    }
    else if (mode == 1)
    {
        _ltc->setCamera(viewMatrix);
        _ltc->setLights({{{1.0f, 1.0f, 1.0f}, lightWorldMatrix}});
        _ltc->render();
    }

    if (ImGui::BeginMainMenuBar())
    {
        ImGui::Text("Arealights (2017)");
        ImGui::EndMainMenuBar();
    }

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

    _clusteringShader->setUniform("TargetTexture", 0);
    _clusteringShader->setUniform("NormalTexture", 1);
    _clusteringShader->setUniform("PositionTexture", 2);

    _renderHelper->drawFullScreenQuad();
}

}
