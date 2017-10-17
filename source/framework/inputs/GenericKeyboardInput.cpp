// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "GenericKeyboardInput.hpp"

namespace fw
{

GenericKeyboardInput::GenericKeyboardInput():
    _keyStates{}
{
}

GenericKeyboardInput::~GenericKeyboardInput()
{
}

void GenericKeyboardInput::keyDown(int key)
{
    if (_keyStates[key] == KeyState::Released)
    {
        _keyStates[key] = KeyState::Tapped;
    }
    else
    {
        _keyStates[key] = KeyState::Pressed;
    }
}

void GenericKeyboardInput::keyUp(int key)
{
    _keyStates[key] = KeyState::Released;
}

bool GenericKeyboardInput::isKeyDown(int key) const
{
    return _keyStates[key] == KeyState::Pressed
        || _keyStates[key] == KeyState::Tapped;
}

bool GenericKeyboardInput::isKeyUp(int key) const
{
    return _keyStates[key] == KeyState::Released;
}

bool GenericKeyboardInput::isKeyTapped(int key) const
{
    return _keyStates[key] == KeyState::Tapped;
}

void GenericKeyboardInput::nextFrame()
{
    changeTapsToPresses();
}

void GenericKeyboardInput::changeTapsToPresses()
{
    for (auto& key: _keyStates)
    {
        if (key == KeyState::Tapped)
        {
            key = KeyState::Pressed;
        }
    }
}

}
