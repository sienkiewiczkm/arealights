// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "framework/ImGuiApplication.hpp"
#include "framework/Vertices.hpp"
#include "framework/Mesh.hpp"
#include "framework/Shaders.hpp"
#include "framework/FreeCamera.hpp"
#include "framework/FreeCameraInputMapper.hpp"
#include "DeferredRenderingPipeline.hpp"
#include "framework/inputs/GenericKeyboardInput.hpp"
#include "framework/Texture.hpp"
#include "framework/inputs/GenericMouseInput.hpp"
#include "RenderHelper.hpp"
#include "LinearlyTransformedCosines.hpp"
#include "PointLightCluster.hpp"
#include "GroundTruth.hpp"
#include "Material.hpp"
#include <memory>
#include "Interface.hpp"

namespace arealights
{

class Application: public fw::ImGuiApplication
{
public:
    Application();
    virtual ~Application();

protected:
    virtual void onCreate() override;
    virtual void onDestroy() override;
    virtual void onUpdate(
        const std::chrono::high_resolution_clock::duration& deltaTime
    ) override;
    virtual void onRender() override;

    virtual bool onKey(int key, int scancode, int action, int mods) override;
    virtual bool onMouseMove(glm::dvec2 newPosition) override;
    virtual bool onMouseButton(int button, int action, int mods) override;

    void preloadShaderInclude(const char *filepath, std::string glslIncludePath) const;
    void loadMaterials();
    void loadShaders();
    void createFramebuffer(const glm::ivec2 &resolution);

    void renderClusters();

private:
    Interface _interface;

    std::shared_ptr<RenderHelper> _renderHelper;

    bool _measure;
    float _measureAccumulator;
    float _measureFramesNum;
    float _lastMeasurement;

    bool _autoscreenshot;
    int _autoscreenshotStep;
    int _autoscreenshotFrame;
    float _timeSum;
    std::ofstream _autoscreenshotOutFile;

    std::string _screenshotAnnotation;

    std::unique_ptr<fw::ShaderProgram> _blitSRGBProgram;
    std::unique_ptr<fw::ShaderProgram> _clusteringShader;

    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _planeMesh;
    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _arealightMesh;

    std::shared_ptr<fw::FreeCamera> _camera;
    FreeCameraInputMapper _cameraInputMapper;

    std::shared_ptr<fw::GenericKeyboardInput> _keyboardInput;
    std::shared_ptr<fw::GenericMouseInput> _mouseInput;

    std::unique_ptr<DeferredRenderingPipeline> _deferredPipeline;

    bool _screenshotRequested;

    unsigned int _intermediateFBO;
    unsigned int _intermediateFBOTexture;

    float _frameTimeMs;

    std::shared_ptr<PointLightCluster> _pointLightCluster;
    std::shared_ptr<LinearlyTransformedCosines> _ltc;
    std::shared_ptr<GroundTruth> _groundTruth;

    int _activeMaterial;
    std::vector<std::pair<std::string, std::shared_ptr<Material>>> _materialMap;

    bool _restartIncrementalRendering;

    bool _cameraLocked;

    long long _frame;

};

}
