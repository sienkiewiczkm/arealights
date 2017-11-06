// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "framework/ImGuiApplication.hpp"
#include "framework/Vertices.hpp"
#include "framework/Mesh.hpp"
#include "framework/Shaders.hpp"
#include "framework/FreeCamera.hpp"
#include "FreeCameraInputMapper.hpp"
#include "DeferredRenderingPipeline.hpp"
#include "ArealightConfigurationUI.hpp"
#include "SceneUI.hpp"
#include "framework/inputs/GenericKeyboardInput.hpp"
#include "framework/Texture.hpp"
#include "framework/inputs/GenericMouseInput.hpp"
#include "RenderHelper.hpp"
#include "ltc/LinearlyTransformedCosines.hpp"
#include "point_light_cluster/PLCConfigurationUI.hpp"
#include "point_light_cluster/PointLightCluster.hpp"
#include "ground_truth/GroundTruth.hpp"
#include "Material.hpp"
#include <memory>

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

    void renderClusters();

private:
    std::shared_ptr<RenderHelper> _renderHelper;

    std::unique_ptr<fw::ShaderProgram> _shaderProgram;
    std::unique_ptr<fw::ShaderProgram> _blitSRGBProgram;
    std::unique_ptr<fw::ShaderProgram> _clusteringShader;

    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _planeMesh;
    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _arealightMesh;

    std::shared_ptr<fw::FreeCamera> _camera;
    FreeCameraInputMapper _cameraInputMapper;

    std::shared_ptr<fw::GenericKeyboardInput> _keyboardInput;
    std::shared_ptr<fw::GenericMouseInput> _mouseInput;

    std::unique_ptr<DeferredRenderingPipeline> _deferredPipeline;

    ArealightConfigurationUI _configurationUI;
    SceneUI _sceneUI;

    unsigned int _intermediateFBO;
    unsigned int _intermediateFBOTexture;

    std::shared_ptr<PointLightCluster> _pointLightCluster;
    std::shared_ptr<LinearlyTransformedCosines> _ltc;
    std::shared_ptr<GroundTruth> _groundTruth;

    std::shared_ptr<Material> _scuffedIronMaterial;
    std::shared_ptr<Material> _copperRockMaterial;
    std::shared_ptr<Material> _woodPlanksMaterial;

    int _activeMaterial;
    std::vector<std::pair<std::string, std::shared_ptr<Material>>> _materialMap;

    PLCConfigurationUI _plcConfigurationUI;

    bool _restartIncrementalRendering;

    void preloadShaderInclude(const char *filepath, std::string glslIncludePath) const;
};

}
