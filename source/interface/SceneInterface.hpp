// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <vector>
#include <string>
#include "IInterfaceView.hpp"

namespace arealights
{

class SceneInterface: public IInterfaceView
{
public:
    SceneInterface();
    virtual ~SceneInterface();

    void render() override;

    void setMaterials(std::vector<std::string> materialNames) { _materialNames = materialNames; }
    int getMaterialId() { return _activeMaterial; }

    int getSceneId() { return _sceneRadio; }
    float getMetalness() { return _metalness; }
    float getRoughness() { return _roughness; }
    void setRoughness(float roughness) { _roughness = roughness; }

private:
    bool _showUI;
    int _sceneRadio;
    float _metalness;
    float _roughness;
    int _activeMaterial;

    int _surfaceType;

    std::vector<std::string> _materialNames;
};

}

