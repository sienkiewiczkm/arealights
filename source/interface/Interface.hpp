#pragma once

#include <memory>
#include <vector>

#include "LightInterface.hpp"
#include "SceneInterface.hpp"
#include "PointLightClusterInterface.hpp"
#include "IInterfaceView.hpp"

class Interface
{
public:
    Interface();
    ~Interface();

    void init();

    void addView(std::shared_ptr<IInterfaceView> view);

    void render();

protected:
    void applyImGuiStyle();

private:
    std::vector<std::shared_ptr<IInterfaceView>> _views;
};
