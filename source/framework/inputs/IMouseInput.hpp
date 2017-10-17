// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include "glm/glm.hpp"
#include "InputStates.hpp"

namespace fw
{

class IMouseInput
{
public:
    virtual ~IMouseInput() {}

    virtual bool isButtonUp(int button) const = 0;
    virtual bool isButtonDown(int button) const = 0;
    virtual bool isButtonTapped(int button) const = 0;

    virtual glm::dvec2 getCurrentPosition() const = 0;
    virtual glm::dvec2 getPreviousPosition() const = 0;
    virtual glm::dvec2 getMovement() const = 0;
};

}
