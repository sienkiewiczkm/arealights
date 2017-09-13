// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "ImGuiApplication.hpp"
#include "Vertices.hpp"
#include "Mesh.hpp"
#include "Shaders.hpp"
#include "FreeCamera.hpp"
#include "FreeCameraInputMapper.hpp"
#include "inputs/GenericKeyboardInput.hpp"
#include "inputs/GenericMouseInput.hpp"
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

private:
    std::unique_ptr<fw::ShaderProgram> _shaderProgram;
    std::unique_ptr<fw::Mesh<fw::StandardVertex3D>> _planeMesh;

    std::shared_ptr<fw::FreeCamera> _camera;
    FreeCameraInputMapper _cameraInputMapper;

    std::shared_ptr<fw::GenericKeyboardInput> _keyboardInput;
    std::shared_ptr<fw::GenericMouseInput> _mouseInput;
};

}
