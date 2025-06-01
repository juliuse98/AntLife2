#pragma once

enum class PheromoneType { toHome, toFood, COUNT_INTERNAL };

struct PheromoneCell {
  float strengths[static_cast<int>(PheromoneType::COUNT_INTERNAL)];
  PheromoneCell() {
    for (int i = 0; i < static_cast<int>(PheromoneType::COUNT_INTERNAL); ++i) {
      strengths[i] = 0.0f;
    }
  }
};
