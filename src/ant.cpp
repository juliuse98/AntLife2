#include "ant.h"

#include <easylogging++.h>

#include "world.h"

#include <cmath>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

namespace al {
void Ant::update(int deltaTime, World &world) {

  glm::vec2 foodLocation = world.getFoodLocation();
  float distanceToFood = glm::distance(foodLocation, position);
  if (food <= 0 && distanceToFood <= ANT_VIEW_DISTANCE) {
    LOG(INFO) << "-=-=-=-=-=-=-=-=-=-=UPDATE=-=-=-=-=-=-=-=-=-=-=-=-=";
    LOG(INFO) << "Food " << food << " distanceToFood " << distanceToFood
              << " ANT VIEW DISTANCE " << ANT_VIEW_DISTANCE;
    LOG(INFO) << "-=-=-=-=-=-=-=-=-=-=CloseToFood=-=-=-=-=-=-=-=-=-=-=-=-=";
    LOG(INFO) << position.x << ", " << position.y;
    if (distanceToFood < 0.5f) {
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      LOG(INFO) << "XOXOXOXOXOXOXO PICK UP FOOD OXOXOXOXOXOXOXOX";
      food++;
    }
    glm::vec2 directionToFood = glm::normalize(foodLocation - position);
    heading = glm::degrees(atan2(directionToFood.x, directionToFood.y));
  }
  // else if going home then check for pheromones
  else {
    float sign = 1 - 2 * (std::fmod(static_cast<int>(position.x) +
                                        static_cast<int>(position.y),
                                    2));
    heading = std::fmod(heading + 1.0f, 360.0f) * sign;
  }

  // Apply direction
  float angle = glm::radians(heading);
  glm::mat4 transform4 = glm::mat4(1.0f);
  transform4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
  glm::mat3 transform = glm::mat3(transform4);
  glm::vec3 movement3 = transform * glm::vec3(0.0f, 1.0f, 0.0f);

  // Apply movement
  glm::vec2 movement2 = glm::vec2(movement3.x, movement3.y);
  position += movement2 * speed;
}
Ant::Ant(float x, float y, float heading)
    : position(x, y), heading(heading), speed(0.1), food(0) {};
} // namespace al
