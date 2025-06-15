#include "ant.h"

#include <easylogging++.h>

#include "glm/ext/vector_float2.hpp"
#include "pheromone_cell.h"
#include "world.h"

#include <cmath>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <random>

namespace al {
void Ant::update(int deltaTime, World &world) {
  glm::vec2 foodLocation = world.getFoodLocation();
  float distanceToFood = glm::distance(foodLocation, position);

  if (false && food <= 0 && distanceToFood <= ANT_VIEW_DISTANCE) {
    LOG(INFO) << "Food " << food << " distanceToFood " << distanceToFood
              << " ANT VIEW DISTANCE " << ANT_VIEW_DISTANCE;
    glm::vec2 directionToFood = glm::normalize(foodLocation - position);
    heading = glm::degrees(atan2(directionToFood.x, directionToFood.y));
  }
  // else if going home then check for pheromones
  else {
    float sign = 1 - 2 * (std::fmod(static_cast<int>(position.x) +
                                        static_cast<int>(position.y),
                                    2));
    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 1);
    sign = dist(gen) * 2 - 1;
    heading = std::fmod(heading + 10.0f * sign, 360.0f);
  }

  // 🧭 Edge avoidance using vector steering
  glm::vec2 steering(0.0f);
  float margin = 10.f;
  float worldWidth = world.getSize().x;
  float worldHeight = world.getSize().y;
  float avoidStrength = 20.0f; // tweak (larger = stronger steering)

  if (position.x < margin) {
    float factor = 1.0f - (position.x / margin);
    steering += glm::vec2(1.0f, 0.0f) * factor * avoidStrength;
  } else if (position.x > worldWidth - margin) {
    float factor = (position.x - (worldWidth - margin)) / margin;
    steering += glm::vec2(-1.0f, 0.0f) * factor * avoidStrength;
  }

  if (position.y < margin) {
    float factor = 1.0f - (position.y / margin);
    steering += glm::vec2(0.0f, 1.0f) * factor * avoidStrength;
  } else if (position.y > worldHeight - margin) {
    float factor = (position.y - (worldHeight - margin)) / margin;
    steering += glm::vec2(0.0f, -1.0f) * factor * avoidStrength;
  }

  // If there's a steering force, blend it into heading
  if (glm::length(steering) > 0.001f) {
    glm::vec2 currentDirection = glm::vec2(std::cos(glm::radians(heading)),
                                           std::sin(glm::radians(heading)));

    // Blend current heading and steering vector
    glm::vec2 newDirection = glm::normalize(
        glm::mix(currentDirection, glm::normalize(steering), 0.1f));
    heading = glm::degrees(std::atan2(newDirection.y, newDirection.x));
  }

  // Apply movement
  float angle = glm::radians(heading);
  glm::mat4 transform4 = glm::mat4(1.0f);
  transform4 = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
  glm::mat3 transform = glm::mat3(transform4);
  glm::vec3 movement3 = transform * glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec2 movement2 = glm::vec2(movement3.x, movement3.y);
  position += movement2 * speed;
}

void Ant::senseAhead(World &world, int &resultHeading) {
  // Three point sensinga
  CellGrid<PheromoneCell> pGrid = world.m_pheromoneGrid;
  glm::vec2 leftSensor;
  glm::vec2 middleSensor;
  glm::vec2 rightSensor;

  float valueLeftSensor =
      pGrid.get(leftSensor.x, leftSensor.y)
          .strengths[static_cast<int>(PheromoneType::toHome)];
  float valueMiddleSensor =
      pGrid.get(middleSensor.x, middleSensor.y)
          .strengths[static_cast<int>(PheromoneType::toHome)];
  float valueRightSensor =
      pGrid.get(rightSensor.x, rightSensor.y)
          .strengths[static_cast<int>(PheromoneType::toHome)];

  if (valueLeftSensor > valueRightSensor &&
      valueLeftSensor > valueMiddleSensor) {
    // Left sensor is the biggest
    resultHeading = 20;
  } else if (valueRightSensor > valueLeftSensor &&
             valueRightSensor > valueMiddleSensor) {
    // Right sensor is the biggest
    resultHeading = 20;
  } else {
    // Middle sensor is the biggest
    resultHeading = 2;
  }
}

Ant::Ant(float x, float y, float heading)
    : position(x, y), heading(heading), speed(1), food(0) {};
} // namespace al
