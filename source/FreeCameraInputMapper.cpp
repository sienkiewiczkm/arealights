// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "FreeCameraInputMapper.hpp"
#include "framework/OpenGL.hpp"
#include "glm/glm.hpp"

namespace arealights
{

FreeCameraInputMapper::FreeCameraInputMapper(): _hadMovement{false}
{
}

void FreeCameraInputMapper::setKeyboardInput(
    std::shared_ptr<fw::IKeyboardInput> kbInput
)
{
    _keyboardInput = kbInput;
}

void FreeCameraInputMapper::setMouseInput(
    std::shared_ptr<fw::IMouseInput> mouseInput
)
{
    _mouseInput = mouseInput;
}

void FreeCameraInputMapper::setCamera(
    std::shared_ptr<fw::FreeCamera> camera
)
{
    _camera = camera;
}

void FreeCameraInputMapper::update(
    const std::chrono::high_resolution_clock::duration& deltaTime
)
{
    auto deltaTimeSeconds = std::chrono::duration<float>(deltaTime);

    _hadMovement = false;

    if (!_camera)
    {
        return;
    }

    if (_keyboardInput && !_locked)
    {
        glm::vec3 movement;

        if (_keyboardInput->isKeyDown(GLFW_KEY_W))
        {
            movement.z += 1.0f;
            _hadMovement = true;
        }

        if (_keyboardInput->isKeyDown(GLFW_KEY_S))
        {
            movement.z -= 1.0f;
            _hadMovement = true;
        }

        if (_keyboardInput->isKeyDown(GLFW_KEY_A))
        {
            movement.x -= 1.0f;
            _hadMovement = true;
        }

        if (_keyboardInput->isKeyDown(GLFW_KEY_D))
        {
            movement.x += 1.0f;
            _hadMovement = true;
        }

        _camera->moveRelatively(deltaTimeSeconds.count() * movement);
    }

    if (_mouseInput && _mouseInput->isButtonDown(0) && !_locked)
    {
        auto movement = _mouseInput->getMovement();
        movement *= deltaTimeSeconds.count();
        _camera->rotate(movement.x, movement.y);
        _hadMovement = true;
    }
}

}
