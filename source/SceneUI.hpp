// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

namespace arealights
{

class SceneUI
{
public:
    SceneUI();
    ~SceneUI();

    void update();

    int getSceneId() { return _sceneRadio; }
    float getRoughness() { return _roughness; }

private:
    bool _showUI;
    int _sceneRadio;
    float _roughness;
};

}

