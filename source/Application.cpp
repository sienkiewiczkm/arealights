// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include <stb_image_write.h>
#include "Application.hpp"
#include "framework/BasicMeshes.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "PointLightCluster.hpp"
#include "framework/Common.hpp"
#include <algorithm>
#include <iomanip>

namespace arealights
{

Application::Application():
    _restartIncrementalRendering{false},
    _activeMaterial{0},
    _screenshotRequested{false},
    _cameraLocked{false},
    _autoscreenshot{false},
    _frame{0},
    _screenshotAnnotation{},
    _measure{},
    _measureAccumulator{},
    _measureFramesNum{},
    _lastMeasurement{}
{
}

Application::~Application()
{
}

void Application::onCreate()
{
    ImGuiApplication::onCreate();
    setWindowSize({1024,768});
    _renderHelper = std::make_shared<RenderHelper>(this);


    // Scene
    auto planeMesh = fw::PlaneGenerator::generate({10.0f, 10.0f});
    _planeMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        planeMesh.vertices, planeMesh.indices
    );

    auto arealightMesh = fw::PlaneGenerator::generate({1.0f, 1.0f});
    _arealightMesh = std::make_unique<fw::Mesh<fw::StandardVertex3D>>(
        arealightMesh.vertices, arealightMesh.indices
    );


    // Shader stuff
    preloadShaderInclude("../assets/shaderlib/gbuffer.glsl", "/gbuffer.glsl");
    preloadShaderInclude("../assets/shaderlib/cookTorrance.glsl", "/cookTorrance.glsl");
    loadShaders();


    // Camera and controls
    _camera = std::make_shared<fw::FreeCamera>();
    _camera->setWorldPosition({0.0f, 1.0f, 5.0f});

    _keyboardInput = std::make_shared<fw::GenericKeyboardInput>();
    _mouseInput = std::make_shared<fw::GenericMouseInput>();

    _cameraInputMapper.setCamera(_camera);
    _cameraInputMapper.setKeyboardInput(_keyboardInput);
    _cameraInputMapper.setMouseInput(_mouseInput);


    // Arealight methods
    _pointLightCluster = std::make_shared<PointLightCluster>(_renderHelper);
    _pointLightCluster->init();

    _groundTruth = std::make_shared<GroundTruth>(_renderHelper);
    _groundTruth->init();

    _ltc = std::make_shared<LinearlyTransformedCosines>(_renderHelper);
    _ltc->init();


    // Misc rendering stuff
    glm::ivec2 resolution{2560,1440};
    _deferredPipeline = std::make_unique<DeferredRenderingPipeline>();
    _deferredPipeline->create(resolution);

    createFramebuffer(resolution);
    loadMaterials();


    // Interface
    _interface.init();


    // Print useful debugging information
    GLint maxTextureUnits;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    LOG(INFO) << "Max texture units: " << maxTextureUnits;

}

void Application::createFramebuffer(const glm::ivec2 &resolution)
{
    glGenFramebuffers(1, &_intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);

    glGenTextures(1, &_intermediateFBOTexture);
    glBindTexture(GL_TEXTURE_2D, _intermediateFBOTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, resolution.x, resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
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

void Application::loadShaders()
{
    _blitSRGBProgram = _renderHelper->makeSimpleShader(
        "../assets/TextureBlitToSRGB.vs.glsl",
        "../assets/TextureBlitToSRGB.fs.glsl"
    );

    _clusteringShader = _renderHelper->makeSimpleShader(
        "../assets/Clustering.vs.glsl",
        "../assets/Clustering.fs.glsl"
    );
}

void Application::loadMaterials()
{
    auto scuffedIronMaterial = std::make_shared<Material>();
    scuffedIronMaterial->setAlbedoTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_basecolor.png");
    scuffedIronMaterial->setNormalTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_normal.png");
    scuffedIronMaterial->setMetalnessTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_metallic.png");
    scuffedIronMaterial->setRoughnessTexture("../assets/textures/Iron-Scuffed/Iron-Scuffed_roughness.png");

    auto marbleCheckerboard = std::make_shared<Material>();
    marbleCheckerboard->setAlbedoTexture("../assets/textures/textures.com/TexturesCom_Marble_Checkerboard_1K_albedo.png");
    marbleCheckerboard->setNormalTexture("../assets/textures/textures.com/TexturesCom_Marble_Checkerboard_1K_normal.png");
    marbleCheckerboard->setMetalnessTexture("../assets/textures/common/metalness_isolator.png");
    marbleCheckerboard->setRoughnessTexture("../assets/textures/textures.com/TexturesCom_Marble_Checkerboard_1K_roughness.png");

    auto copperPolished = std::make_shared<Material>();
    copperPolished->setAlbedoTexture("../assets/textures/textures.com/TexturesCom_Metal_CopperPolished_1K_albedo.png");
    copperPolished->setNormalTexture("../assets/textures/textures.com/TexturesCom_Metal_CopperPolished_1K_normal.png");
    copperPolished->setMetalnessTexture("../assets/textures/textures.com/TexturesCom_Metal_CopperPolished_1K_metallic.png");
    copperPolished->setRoughnessTexture("../assets/textures/textures.com/TexturesCom_Metal_CopperPolished_1K_roughness.png");

    auto polypropylene = std::make_shared<Material>();
    polypropylene->setAlbedoTexture("../assets/textures/textures.com/TexturesCom_Plastic_PolypropyleneRough_1K_albedo.png");
    polypropylene->setNormalTexture("../assets/textures/textures.com/TexturesCom_Plastic_PolypropyleneRough_1K_normal.png");
    polypropylene->setMetalnessTexture("../assets/textures/common/metalness_isolator.png");
    polypropylene->setRoughnessTexture("../assets/textures/textures.com/TexturesCom_Plastic_PolypropyleneRough_1K_roughness.png");

    auto tilesCheckerboard = std::make_shared<Material>();
    tilesCheckerboard->setAlbedoTexture("../assets/textures/textures.com/TexturesCom_Tiles_CheckerboardVintage_1K_albedo.png");
    tilesCheckerboard->setNormalTexture("../assets/textures/textures.com/TexturesCom_Tiles_CheckerboardVintage_1K_normal.png");
    tilesCheckerboard->setMetalnessTexture("../assets/textures/common/metalness_isolator.png");
    tilesCheckerboard->setRoughnessTexture("../assets/textures/textures.com/TexturesCom_Tiles_CheckerboardVintage_1K_roughness.png");

    _materialMap.push_back({"Scuffed Iron", scuffedIronMaterial});
    _materialMap.push_back({"Marble checkerboard", marbleCheckerboard});
    _materialMap.push_back({"Polished copper", copperPolished});
    _materialMap.push_back({"Polypropylene", polypropylene});
    _materialMap.push_back({"Vintage tiles", tilesCheckerboard});
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
    ++_frame;

    ImGuiApplication::onUpdate(deltaTime);

    _cameraInputMapper.setLock(_cameraLocked);
    _cameraInputMapper.update(deltaTime);
    _restartIncrementalRendering = _cameraInputMapper.hadMovement();

    _interface.render();

    _restartIncrementalRendering = _cameraInputMapper.hadMovement();

    std::vector<std::string> materialNames;
    std::transform(std::begin(_materialMap), std::end(_materialMap), std::back_inserter(materialNames),
        [](const std::pair<std::string, std::shared_ptr<Material>> &p) { return p.first; });

    _interface.setMaterials(materialNames);

    if (_keyboardInput->isKeyTapped(GLFW_KEY_P)) {
        _screenshotRequested = true;
        _screenshotAnnotation = "";
    }

    if (_keyboardInput->isKeyTapped(GLFW_KEY_I)) {
        _restartIncrementalRendering = true;
    }

    if (_keyboardInput->isKeyTapped(GLFW_KEY_L)) {
        _cameraLocked = !_cameraLocked;
    }

    // Automate screenshot taking

    if (_keyboardInput->isKeyTapped(GLFW_KEY_1)) {
        this->setWindowSize({1024, 1024});
    }

    if (_keyboardInput->isKeyTapped(GLFW_KEY_2)) {
        _camera->setEulerAngles({M_PI*(1.0/12.0), 0, 0});
        _camera->setWorldPosition({0, 0.5, 5});
    }

    if (_keyboardInput->isKeyTapped(GLFW_KEY_3)) {
        _camera->setEulerAngles({0, -M_PI_4, 0});
        _camera->setWorldPosition({5, 0.3, 5});
    }

    if (_keyboardInput->isKeyTapped(GLFW_KEY_5)) {
        _autoscreenshot = true;
        _autoscreenshotStep = 0;
        _autoscreenshotFrame = 0;
    }

    /*
    // Average time measurements
    if (_keyboardInput->isKeyTapped(GLFW_KEY_M)) {
        _measure = true;
        _measureAccumulator = 0.0f;
        _measureFramesNum = 0;
    }

    if (_measure) {
        if (_measureFramesNum < 100) {
            _measureAccumulator += _frameTimeMs;
            ++_measureFramesNum;
        } else {
            _measure = false;
            _lastMeasurement = _measureAccumulator / 100.0f;
        }
    }

    ImGui::Text("Last measurement: %.4f", _lastMeasurement);
    */


    // Autoscreenshoting for paper purposes
    if (_autoscreenshot) {
        if (_autoscreenshotStep <= 5) {
            if (_autoscreenshotStep == 0 && _autoscreenshotFrame == 0) {
                _autoscreenshotOutFile.open("autoscreenshot_log.txt", std::ios_base::app);
            }

            const int numFrames = _interface.getArealightMethod() == AREALIGHT_GROUNDTRUTH ? 1000 : 5;

            _timeSum += _frameTimeMs;

            // finalize previous step - all frames done for this one
            if (_autoscreenshotFrame == numFrames * (_autoscreenshotStep+1)) {
                // log time
                _autoscreenshotOutFile << "method=" << static_cast<int>(_interface.getArealightMethod());
                _autoscreenshotOutFile << "\rroughness=" << std::max(0.05f, _autoscreenshotStep * 0.2f);
                _autoscreenshotOutFile << "\rtime=" << (_interface.getArealightMethod() == AREALIGHT_GROUNDTRUTH ?
                    _timeSum :
                    _timeSum / numFrames) << std::endl;
                // go to next step
                ++_autoscreenshotStep;
            }

            // first frame of the new step
            if (_autoscreenshotStep <= 5 && _autoscreenshotFrame == numFrames * _autoscreenshotStep) {
                _interface.setRoughness(std::max(0.05f, _autoscreenshotStep * 0.2f));
                _timeSum = 0.0;
                _restartIncrementalRendering = true;
            }

            // ground truth needs a few frames, that's why there is a waiting period, also for other methods we
            // calculate average for gen time
            // last frame of current step
            if (_autoscreenshotFrame == numFrames * (_autoscreenshotStep + 1) - 1) {
                _screenshotRequested = true;
            }

            ++_autoscreenshotFrame;
        } else {
            _autoscreenshot = false;
            if (_autoscreenshotOutFile.is_open()) {
                _autoscreenshotOutFile << "Closing.";
                _autoscreenshotOutFile.close();
            }
        }
    }
}

void Application::onRender()
{
    auto framebufferSize = getFramebufferSize();
    auto aspectRatio = static_cast<float>(framebufferSize.x) / framebufferSize.y;

    auto frameRenderingStart = std::chrono::high_resolution_clock::now();

    auto viewMatrix = _camera->getViewMatrix();
    auto projMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    _deferredPipeline->startGeometryPass();

    glEnable(GL_DEPTH_TEST);

    _deferredPipeline->setModelMatrix({});
    _deferredPipeline->setViewMatrix(viewMatrix);
    _deferredPipeline->setProjectionMatrix(projMatrix);
    _deferredPipeline->setMaterialID(0.1f);

    _materialMap[_interface.getMaterialId()].second->bind();

    _deferredPipeline->getShader()->setUniform("SolidMode", _interface.getSceneId());
    _deferredPipeline->getShader()->setUniform("MetalnessConst", _interface.getMetalness());
    _deferredPipeline->getShader()->setUniform("RoughnessConst", _interface.getRoughness());

    _planeMesh->render();

    auto lightWorldMatrix = glm::translate({}, _interface.getPosition()) *
        glm::rotate(
            glm::mat4{},
            glm::radians(_interface.getRotation().x),
            glm::vec3{1.0f, 0.0f, 0.0f}
        ) *
        glm::rotate(
            glm::mat4{},
            glm::radians(_interface.getRotation().y),
            glm::vec3{0.0f, 1.0f, 0.0f}
        ) *
        glm::scale({}, glm::vec3{_interface.getSize(), 1.0f});

    auto planeUpMatrix = glm::rotate(glm::mat4{}, glm::radians(90.0f), glm::vec3{1.0f, 0.0f, 0.0f});
    _deferredPipeline->setModelMatrix(lightWorldMatrix * planeUpMatrix);
    _deferredPipeline->setViewMatrix(viewMatrix);
    _deferredPipeline->setProjectionMatrix(projMatrix);
    _deferredPipeline->setMaterialID(0.3f);
    _arealightMesh->render();

    _deferredPipeline->endGeometryPass();

    auto gbufferResolution = _deferredPipeline->getFramebufferSize();

    auto mode = _interface.getArealightMethod();

    glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);
    glViewport(0, 0, gbufferResolution.x, gbufferResolution.y);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLbitfield clearMask = GL_DEPTH_BUFFER_BIT;

    const auto isIncremental = (mode == AREALIGHT_GROUNDTRUTH);

    if (mode == AREALIGHT_GROUNDTRUTH && _restartIncrementalRendering) {
        _groundTruth->restartIncrementalRendering();
        LOG(INFO) << "Restarting incremental rendering";
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
        _ltc->setFlux(_interface.getFlux());
        _ltc->setCamera(viewMatrix);
        _ltc->setLights({{{1.0f, 1.0f, 1.0f}, lightWorldMatrix}});
        _ltc->render();
    }
    else if (mode == AREALIGHT_CLUSTER)
    {
        _pointLightCluster->setLightFlux(_interface.getFlux());
        _pointLightCluster->setClusterSize(_interface.getClusterSize());
        _pointLightCluster->setCamera(viewMatrix, projMatrix);
        _pointLightCluster->setLights({{{1.0f, 1.0f, 1.0f}, lightWorldMatrix}});
        _pointLightCluster->render();
    }
    else if (mode == AREALIGHT_GROUNDTRUTH)
    {
        auto lightSize = _interface.getSize();
        auto lightArea = lightSize.x * lightSize.y;
        _groundTruth->setRadiosity(_interface.getFlux() / lightArea);
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

    auto frameRenderingEnd = std::chrono::high_resolution_clock::now();
    _frameTimeMs = std::chrono::duration<float>(frameRenderingEnd - frameRenderingStart).count() * 1000;

    if (_frame % 20 == 0) {
        _interface.setRenderTimeMs(_frameTimeMs);
    }

    if (_screenshotRequested) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::string methodName = "";
        switch (_interface.getArealightMethod()) {
        case AREALIGHT_DISABLED:
            methodName = "disabled";
            break;
        case AREALIGHT_CLUSTER:
        {
            std::stringstream ss;
            auto clusterSize = _pointLightCluster->getClusterSize();
            ss << "point" << clusterSize.x << "x" << clusterSize.y;
            methodName = ss.str();
            break;
        }
        case AREALIGHT_GROUNDTRUTH:
            methodName = "gt";
            break;
        case AREALIGHT_LTC:
            methodName = "ltc";
            break;
        }

        std::stringstream ss;
        ss << methodName;
        ss << "-flux-" << _interface.getFlux();
        ss << "-roughness-" << _interface.getRoughness();
        ss << std::put_time(std::localtime(&in_time_t), "-%Y-%m-%d-%H-%M-%S");
        if (_screenshotAnnotation.length() > 0) {
            ss << "-" << _screenshotAnnotation;
        }
        ss << ".png";

        int stride = 3 * sizeof(unsigned char) * framebufferSize.x;
        std::vector<unsigned char> data(3 * framebufferSize.x * framebufferSize.y);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, framebufferSize.x, framebufferSize.y, GL_RGB, GL_UNSIGNED_BYTE, data.data());
        std::vector<unsigned char> orig = data;

        // We have to flip image to save it correctly.
        for (int y = 0; y < framebufferSize.y/2; ++y) {
            for (int x = 0; x < framebufferSize.x; ++x) {
                auto sourceBase = 3 * (framebufferSize.x * y + x);
                auto targetBase = 3 * (framebufferSize.x * (framebufferSize.y - y - 1) + x);
                for (int i = 0; i < 3; ++i) {
                    std::swap(data[sourceBase + i], data[targetBase + i]);
                }
            }
        }

        stbi_write_png(ss.str().c_str(), framebufferSize.x, framebufferSize.y, 3, data.data(), stride);

        LOG(INFO) << "Screenshot saved in " << ss.str() << " " << framebufferSize.x << "x" << framebufferSize.y;

        _screenshotRequested = false;
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

    _clusteringShader->setUniform("GBufferC", 0);
    _clusteringShader->setUniform("GBufferB", 1);
    _clusteringShader->setUniform("GBufferA", 2);

    _renderHelper->drawFullScreenQuad();
}

}
