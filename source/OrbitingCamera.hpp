// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <glm/mat4x4.hpp>

namespace fw
{

class OrbitingCamera
{
public:
  OrbitingCamera();

  inline void setMinXRotation(float min) { _xMin = min; }
  inline void setMaxXRotation(float max) { _xMax = max; }

  inline float getDist() { return _dist; }
  inline void setDist(float dist) { _dist = dist; }

  void rotate(float dx, float dy);

  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const;

private:
  float _xMin, _xMax;
  float _dist;
  float _rotationX, _rotationY;
};

}
