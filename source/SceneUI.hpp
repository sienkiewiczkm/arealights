// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <vector>
#include <string>

namespace arealights
{

class SceneUI
{
public:
    SceneUI();
    ~SceneUI();

    void update();

    void setMaterials(std::vector<std::string> materialNames) { _materialNames = materialNames; }
    int getMaterialId() { return _activeMaterial; }

    int getSceneId() { return _sceneRadio; }
    float getRoughness() { return _roughness; }

private:
    bool _showUI;
    int _sceneRadio;
    float _roughness;
    int _activeMaterial;

    std::vector<std::string> _materialNames;
};

}

