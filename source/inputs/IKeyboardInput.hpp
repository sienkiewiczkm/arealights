// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "InputStates.hpp"

namespace fw
{

class IKeyboardInput
{
public:
    virtual ~IKeyboardInput() {}

    virtual bool isKeyDown(int key) const = 0;
    virtual bool isKeyUp(int key) const = 0;
    virtual bool isKeyTapped(int key) const = 0;
};

}
