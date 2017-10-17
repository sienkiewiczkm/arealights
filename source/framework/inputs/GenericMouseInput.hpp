// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "IMouseInput.hpp"

namespace fw
{

class GenericMouseInput:
    public IMouseInput
{
public:
    GenericMouseInput();
    explicit GenericMouseInput(const glm::dvec2& initialMousePosition);
    virtual ~GenericMouseInput();

    void buttonUp(int button);
    void buttonDown(int button);
    void move(glm::dvec2 newPosition);

    virtual bool isButtonUp(int button) const override;
    virtual bool isButtonDown(int button) const override;
    virtual bool isButtonTapped(int button) const override;

    virtual glm::dvec2 getCurrentPosition() const override;
    virtual glm::dvec2 getPreviousPosition() const override;
    virtual glm::dvec2 getMovement() const override;

    /// Should be called at the very end of each frame before arrival of the
    /// new events.
    void nextFrame();

private:
    void changeTapsToPresses();

    KeyState _buttonStates[32];
    glm::dvec2 _previousPosition;
    glm::dvec2 _currentPosition;
    glm::dvec2 _movement;
};

}
