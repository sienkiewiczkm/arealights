// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include "OpenGLApplication.hpp"

namespace fw
{

class ImGuiApplication:
    public OpenGLApplication
{
public:
    ImGuiApplication();
    virtual ~ImGuiApplication();

protected:
    virtual void onCreate() override;
    virtual void onDestroy() override;
    virtual void onUpdate(
        const std::chrono::high_resolution_clock::duration& deltaTime
    ) override;
    virtual void onRender() override;

    virtual bool onMouseButton(int button, int action, int mods) override;
    virtual bool onScroll(double xoffset, double yoffset) override;
    virtual bool onKey(int key, int scancode, int action, int mods) override;
    virtual bool onChar(unsigned int c) override;
};

}
