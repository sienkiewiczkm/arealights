// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "ImGuiApplication.hpp"
#include "Vertices.hpp"
#include "Mesh.hpp"
#include "Shaders.hpp"
#include "FreeCamera.hpp"
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

private:
    std::unique_ptr<fw::ShaderProgram> _shaderProgram;
    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _planeMesh;

    fw::FreeCamera _camera;
};

}
