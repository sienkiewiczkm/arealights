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

    _camera.setWorldPosition({0.0f, 1.0f, 5.0f});
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
}

void Application::onRender()
{
    glClearColor(0.007f, 0.11f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shaderProgram->use();

    auto viewMtxLoc = _shaderProgram->getUniformLoc("viewMatrix");
    _shaderProgram->setUniform(viewMtxLoc, _camera.getViewMatrix());

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
}

}
