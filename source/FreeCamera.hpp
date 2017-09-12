// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace fw
{

class FreeCamera
{
public:
    FreeCamera();

    void setWorldPosition(const glm::vec3& worldPosition);
    void setEulerAngles(const glm::vec3& eulerAngles);

    void move(const glm::vec3& movement);
    void moveRelatively(const glm::vec3& movement);
    void rotate(float yaw, float pitch);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getTranslationMatrix() const;
    glm::mat4 getRotationMatrix() const;

    glm::vec3 getWorldForwardDirection() const;
    glm::vec3 getWorldRightDirection() const;
    glm::vec3 getWorldUpDirection() const;

private:
    glm::vec3 _worldPosition;
    glm::vec3 _eulerAngles;
    glm::vec3 _upDirection;
};

}
