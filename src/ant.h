#pragma once
#include <glm/glm.hpp>

#define ANT_VIEW_DISTANCE 3.0f

namespace al {
class World;
struct Ant {
public:
  void update(int deltaTime, World &world);
  Ant(float x, float y, float heading);

  glm::vec2 position;
  float heading; // in degrees; 0 is towards top
  float speed;
  float food;
};
} // namespace al
