// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include <memory>
#include <chrono>
#include "inputs/IKeyboardInput.hpp"
#include "inputs/IMouseInput.hpp"
#include "FreeCamera.hpp"

namespace arealights
{

class FreeCameraInputMapper
{
public:
    FreeCameraInputMapper();

    void setKeyboardInput(std::shared_ptr<fw::IKeyboardInput> kbInput);
    void setMouseInput(std::shared_ptr<fw::IMouseInput> mouseInput);
    void setCamera(std::shared_ptr<fw::FreeCamera> camera);

    void update(const std::chrono::high_resolution_clock::duration& deltaTime);

private:
    std::shared_ptr<fw::FreeCamera> _camera;
    std::shared_ptr<fw::IKeyboardInput> _keyboardInput;
    std::shared_ptr<fw::IMouseInput> _mouseInput;
};

}
