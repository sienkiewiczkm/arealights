#pragma once

#include "glm/glm.hpp"
#include <memory>
#include <vector>

enum EArealightMethod {
    AREALIGHT_DISABLED = 0,
    AREALIGHT_CLUSTER = 1,
    AREALIGHT_GROUNDTRUTH = 2,
    AREALIGHT_LTC = 3
};

class Interface
{
public:
    Interface();
    ~Interface();

    void init();
    void render();

    // General
    void setRenderTimeMs(float ms) { _renderTimeMs = ms; }

    // Scene
    void setMaterials(std::vector<std::string> materialNames) { _materialNames = materialNames; }
    int getMaterialId() { return _activeMaterial; }
    int getSceneId() { return _sceneRadio; }
    float getMetalness() { return _metalness; }
    float getRoughness() { return _roughness; }
    void setRoughness(float roughness) { _roughness = roughness; }

    // Point light cluster
    glm::ivec2 getClusterSize() { return _clusterSize; }

    // Arealight
    const glm::vec3 getPosition() { return _position; }
    const glm::vec2 getSize() { return _size; }
    const glm::vec2 getRotation() { return _rotation; }
    const float getFlux() { return _flux; }
    const EArealightMethod getArealightMethod() { return _selectedArealightMethod; };

protected:
    void applyImGuiStyle();

    void renderSceneSettings();
    void renderLightSettings();

private:
    float _renderTimeMs;

    // Scene
    int _sceneRadio;
    float _metalness;
    float _roughness;
    int _activeMaterial;
    int _surfaceType;
    std::vector<std::string> _materialNames;

    // Point light cluster
    bool _oneWay;
    glm::ivec2 _clusterSize;

    // Arealight settings
    EArealightMethod _selectedArealightMethod;
    glm::vec3 _position;
    glm::vec2 _size;
    glm::vec2 _rotation;
    float _flux;
};
