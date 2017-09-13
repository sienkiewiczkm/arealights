// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Application.hpp"
#include "BasicMeshes.cpp"
#include "glm/gtc/matrix_transform.hpp"

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

    auto planeMesh = fw::PlaneGenerator::generate({1.0f, 1.0f});
    _planeMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        planeMesh.vertices, planeMesh.indices
    );

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

    _camera = std::make_shared<fw::FreeCamera>();
    _camera->setWorldPosition({0.0f, 1.0f, 5.0f});

    _keyboardInput = std::make_shared<fw::GenericKeyboardInput>();
    _mouseInput = std::make_shared<fw::GenericMouseInput>();

    _cameraInputMapper.setCamera(_camera);
    _cameraInputMapper.setKeyboardInput(_keyboardInput);
    _cameraInputMapper.setMouseInput(_mouseInput);
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
    glClearColor(0.007f, 0.11f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shaderProgram->use();

    auto viewMtxLoc = _shaderProgram->getUniformLoc("viewMatrix");
    _shaderProgram->setUniform(viewMtxLoc, _camera->getViewMatrix());

    auto projMtx = glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );

    auto projMtxLoc = _shaderProgram->getUniformLoc("projMatrix");
    _shaderProgram->setUniform(projMtxLoc, projMtx);

    _planeMesh->render();

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

}
