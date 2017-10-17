// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "FreeCamera.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace fw
{

FreeCamera::FreeCamera():
    _upDirection{0.0f, 1.0f, 0.0f}
{
}

void FreeCamera::setWorldPosition(const glm::vec3& worldPosition)
{
    _worldPosition = worldPosition;
}

void FreeCamera::setEulerAngles(const glm::vec3& eulerAngles)
{
    _eulerAngles = eulerAngles;
}

void FreeCamera::move(const glm::vec3& movement)
{
    _worldPosition += movement;
}

void FreeCamera::moveRelatively(const glm::vec3& movement)
{
    auto relativeMovement =
        movement.z * getWorldForwardDirection()
        + movement.y * getWorldUpDirection()
        + movement.x * getWorldRightDirection();

    _worldPosition += relativeMovement;
}

void FreeCamera::rotate(float yaw, float pitch)
{
    _eulerAngles.y += yaw;
    _eulerAngles.x += pitch;
}

glm::mat4 FreeCamera::getViewMatrix() const
{
    return getRotationMatrix() * getTranslationMatrix();
}

glm::mat4 FreeCamera::getTranslationMatrix() const
{
    return glm::translate({}, -_worldPosition);
}

glm::mat4 FreeCamera::getRotationMatrix() const
{
    auto yawRotation = glm::rotate(
        glm::mat4{},
        _eulerAngles.y,
        glm::vec3{0.0f, 1.0f, 0.0f}
    );

    auto pitchRotation = glm::rotate(
        glm::mat4{},
        _eulerAngles.x,
        glm::vec3{1.0f, 0.0f, 0.0f}
    );

    return pitchRotation * yawRotation;
}

glm::vec3 FreeCamera::getWorldForwardDirection() const
{
    auto invRotation = glm::inverse(getRotationMatrix());
    return glm::normalize(
        glm::vec3{invRotation * glm::vec4{0.0f, 0.0f, -1.0f, 0.0f}}
    );
}

glm::vec3 FreeCamera::getWorldRightDirection() const
{
    return glm::normalize(
        glm::cross(getWorldForwardDirection(), getWorldUpDirection())
    );
}

glm::vec3 FreeCamera::getWorldUpDirection() const
{
    return _upDirection;
}

}
