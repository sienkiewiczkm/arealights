// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "IKeyboardInput.hpp"

namespace fw
{

class GenericKeyboardInput:
    public IKeyboardInput
{
public:
    GenericKeyboardInput();
    virtual ~GenericKeyboardInput();

    void keyDown(int key);
    void keyUp(int key);

    virtual bool isKeyDown(int key) const override;
    virtual bool isKeyUp(int key) const override;
    virtual bool isKeyTapped(int key) const override;

    /// Should be called at the very end of each frame before arrival of the
    /// new events.
    void nextFrame();

private:
    void changeTapsToPresses();

    KeyState _keyStates[512];
};

}
