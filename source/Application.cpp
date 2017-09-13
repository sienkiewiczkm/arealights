// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Application.hpp"
#include "BasicMeshes.cpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

namespace arealights
{

Application::Application():
    _quadVAO{0},
    _quadVBO{0}
{
}

Application::~Application()
{
}

void Application::onCreate()
{
    ImGuiApplication::onCreate();

    auto planeMesh = fw::PlaneGenerator::generate({1.0f, 1.0f});
    _planeMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        planeMesh.vertices, planeMesh.indices
    );

    {
        fw::Shader vertexShader;
        vertexShader.addSourceFromFile("../assets/DebugShader.vs");
        vertexShader.compile(GL_VERTEX_SHADER);

        fw::Shader fragmentShader;
        fragmentShader.addSourceFromFile("../assets/DebugShader.fs");
        fragmentShader.compile(GL_FRAGMENT_SHADER);

        _shaderProgram = std::make_unique<fw::ShaderProgram>();
        _shaderProgram->attach(&vertexShader);
        _shaderProgram->attach(&fragmentShader);
        _shaderProgram->link();
    }

    {
        fw::Shader vertexShader, fragmentShader;

        vertexShader.addSourceFromFile("../assets/TextureBlit.vs");
        vertexShader.compile(GL_VERTEX_SHADER);

        fragmentShader.addSourceFromFile("../assets/TextureBlit.fs");
        fragmentShader.compile(GL_FRAGMENT_SHADER);

        _textureBlitShader = std::make_unique<fw::ShaderProgram>();
        _textureBlitShader->attach(&vertexShader);
        _textureBlitShader->attach(&fragmentShader);
        _textureBlitShader->link();
    }

    _camera = std::make_shared<fw::FreeCamera>();
    _camera->setWorldPosition({0.0f, 1.0f, 5.0f});

    _keyboardInput = std::make_shared<fw::GenericKeyboardInput>();
    _mouseInput = std::make_shared<fw::GenericMouseInput>();

    _cameraInputMapper.setCamera(_camera);
    _cameraInputMapper.setKeyboardInput(_keyboardInput);
    _cameraInputMapper.setMouseInput(_mouseInput);

    _deferredPipeline = std::make_unique<DeferredRenderingPipeline>();
    _deferredPipeline->create({1600, 1200});
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
}

void Application::onRender()
{
    auto framebufferSize = getFramebufferSize();
    auto aspectRatio = static_cast<float>(framebufferSize.x) / framebufferSize.y;

    auto viewMatrix = _camera->getViewMatrix();
    auto projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    _deferredPipeline->startGeometryPass();
    _deferredPipeline->setViewMatrix(viewMatrix);
    _deferredPipeline->setProjectionMatrix(projMatrix);
    _planeMesh->render();
    _deferredPipeline->endGeometryPass();

    // this will be done once valid architecture shape will emerge
    _deferredPipeline->startLightingPass();
    _deferredPipeline->endLightingPass();

    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClearColor(0.007f, 0.11f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureBlitShader->use();

    auto targetTextureLoc = _textureBlitShader->getUniformLoc("TargetTexture");
    _textureBlitShader->setUniform(targetTextureLoc, 0);

    auto colorBuffer = _deferredPipeline->getColorBuffer();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

    renderQuad();

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

void Application::renderQuad()
{
    if (_quadVAO == 0)
    {
        LOG(INFO) << "Generating renderable quad.";

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

}
