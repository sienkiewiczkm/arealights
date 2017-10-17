// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "GenericMouseInput.hpp"

namespace fw
{

GenericMouseInput::GenericMouseInput():
    _buttonStates{},
    _previousPosition{},
    _currentPosition{},
    _movement{}
{
}

GenericMouseInput::GenericMouseInput(const glm::dvec2& initialMousePosition):
    _buttonStates{},
    _previousPosition{initialMousePosition},
    _currentPosition{initialMousePosition},
    _movement{}
{
}


GenericMouseInput::~GenericMouseInput()
{
}

void GenericMouseInput::buttonUp(int button)
{
    _buttonStates[button] = KeyState::Released;
}

void GenericMouseInput::buttonDown(int button)
{
    if (_buttonStates[button] == KeyState::Released)
    {
        _buttonStates[button] = KeyState::Tapped;
    }
    else
    {
        _buttonStates[button] = KeyState::Pressed;
    }
}

void GenericMouseInput::move(glm::dvec2 newPosition)
{
    _previousPosition = _currentPosition;
    _currentPosition = newPosition;
    _movement = _currentPosition - _previousPosition;
}


bool GenericMouseInput::isButtonUp(int button) const
{
    return _buttonStates[button] == KeyState::Released;
}

bool GenericMouseInput::isButtonDown(int button) const
{
    return _buttonStates[button] == KeyState::Pressed
        || _buttonStates[button] == KeyState::Tapped;
}

bool GenericMouseInput::isButtonTapped(int button) const
{
    return _buttonStates[button] == KeyState::Tapped;
}

glm::dvec2 GenericMouseInput::getCurrentPosition() const
{
    return _currentPosition;
}

glm::dvec2 GenericMouseInput::getPreviousPosition() const
{
    return _previousPosition;
}

glm::dvec2 GenericMouseInput::getMovement() const
{
    return _movement;
}

void GenericMouseInput::nextFrame()
{
    changeTapsToPresses();
    _previousPosition = _currentPosition;
    _movement = _currentPosition - _previousPosition;
}

void GenericMouseInput::changeTapsToPresses()
{
    for (auto& key: _buttonStates)
    {
        if (key == KeyState::Tapped)
        {
            key = KeyState::Pressed;
        }
    }
}

}

