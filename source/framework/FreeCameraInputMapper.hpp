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

/***
 * \brief This module maps keyboard and mouse actions to FreeCamera movement. Mapping is very simple and it is targeted
 *        towards debug purposes.
 *
 */
class FreeCameraInputMapper
{
public:
    FreeCameraInputMapper();

    void setKeyboardInput(std::shared_ptr<fw::IKeyboardInput> kbInput);
    void setMouseInput(std::shared_ptr<fw::IMouseInput> mouseInput);
    void setCamera(std::shared_ptr<fw::FreeCamera> camera);

    void update(const std::chrono::high_resolution_clock::duration& deltaTime);
    bool hadMovement() const { return _hadMovement; }

    void setLock(bool locked) { _locked = locked; }

private:
    std::shared_ptr<fw::FreeCamera> _camera;
    std::shared_ptr<fw::IKeyboardInput> _keyboardInput;
    std::shared_ptr<fw::IMouseInput> _mouseInput;
    bool _hadMovement;
    bool _locked;
};

}
