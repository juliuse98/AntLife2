#pragma once
#include <glm/glm.hpp>

#define ANT_VIEW_DISTANCE 3.0f
#define ANT_SENSE_DISTANCE 2.0f
#define ANT_ANTENNA_ANGLE 45.0f
#define ANT_MIN_PHEROMONE_STRENGTH 8.0f

namespace al {
class World;
struct Ant {
public:
  void update(int deltaTime, World &world);
  Ant(float x, float y, float heading);
  void senseAhead(World &world, int &resultHeading);

  glm::vec2 position;
  float heading; // in degrees; 0 is towards top
  float speed;
  float food;
};
} // namespace al
