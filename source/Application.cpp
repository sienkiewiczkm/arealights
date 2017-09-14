// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Application.hpp"
#include "BasicMeshes.cpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "ltc.inc"

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

    auto planeMesh = fw::PlaneGenerator::generate({10.0f, 10.0f});
    _planeMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        planeMesh.vertices, planeMesh.indices
    );

    auto arealightMesh = fw::PlaneGenerator::generate({1.0f, 1.0f});
    _arealightMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        arealightMesh.vertices, arealightMesh.indices
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

    {
        fw::Shader vertexShader, fragmentShader;

        vertexShader.addSourceFromFile("../assets/LTC.vs");
        vertexShader.compile(GL_VERTEX_SHADER);

        fragmentShader.addSourceFromFile("../assets/LTC.fs");
        fragmentShader.compile(GL_FRAGMENT_SHADER);

        _ltcShader = std::make_unique<fw::ShaderProgram>();
        _ltcShader->attach(&vertexShader);
        _ltcShader->attach(&fragmentShader);
        _ltcShader->link();
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

    loadLookupTextures();
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

    glViewport(0, 0, framebufferSize.x, framebufferSize.y);
    glClearColor(0.007f, 0.11f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _ltcShader->use();

    auto targetTextureLoc = _ltcShader->getUniformLoc("TargetTexture");
    _ltcShader->setUniform(targetTextureLoc, 0);

    auto normalTextureLoc = _ltcShader->getUniformLoc("NormalTexture");
    _ltcShader->setUniform(normalTextureLoc, 1);

    auto positionTextureLoc = _ltcShader->getUniformLoc("PositionTexture");
    _ltcShader->setUniform(positionTextureLoc, 2);

    auto ltcMatLoc = _ltcShader->getUniformLoc("ltc_mat");
    _ltcShader->setUniform(ltcMatLoc, 3);

    auto ltcMagLoc = _ltcShader->getUniformLoc("ltc_mag");
    _ltcShader->setUniform(ltcMagLoc, 4);

    auto viewMatrixLoc = _ltcShader->getUniformLoc("viewMatrix");
    _ltcShader->setUniform(viewMatrixLoc, viewMatrix);

    auto arealightMatLoc = _ltcShader->getUniformLoc("arealightTransform");
    _ltcShader->setUniform(arealightMatLoc, lightWorldMatrix);

    auto positionBuffer = _deferredPipeline->getPositionBuffer();
    auto normalBuffer = _deferredPipeline->getNormalBuffer();
    auto colorBuffer = _deferredPipeline->getColorBuffer();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalBuffer);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, positionBuffer);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _ltcMat);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _ltcMag);

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

void Application::loadLookupTextures()
{
    glGenTextures(1, &_ltcMat);
    glGenTextures(1, &_ltcMag);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _ltcMat);

    std::vector<float> textureData;
    textureData.resize(4 * ltclookup::size * ltclookup::size);

    for (auto i = 0; i < ltclookup::size; ++i)
    {
        for (auto j = 0; j < ltclookup::size; ++j)
        {
            auto matIdx = ltclookup::size * j + i;
            auto base = 4 * matIdx;
            const auto& mat = ltclookup::tabM[matIdx];

            const auto& a = mat[0][0];
            const auto& b = mat[0][2];
            const auto& c = mat[1][1];
            const auto& d = mat[2][0];

            textureData[base+0] = a;
            textureData[base+1] = -b;
            textureData[base+2] = (a-b*d)/c;
            textureData[base+3] = -d;
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        ltclookup::size,
        ltclookup::size,
        0,
        GL_RGBA,
        GL_FLOAT,
        textureData.data()
    );

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "ERROR: " << err << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, _ltcMat);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_R,
        ltclookup::size,
        ltclookup::size,
        0,
        GL_R,
        GL_FLOAT,
        ltclookup::tabAmplitude
    );
}

}
