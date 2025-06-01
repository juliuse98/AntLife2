#pragma once
#include <easylogging++.h>

#include "cell_grid.h"
#include "pheromone_cell.h"
#include <atomic>
#include <glm/glm.hpp>
#include <mutex>
#include <thread>
#include <vector>

#define ANTS_PER_THREAD 1000
namespace al {
class Ant;
class World {
public:
  World(int antCount);
  ~World();

  void update();
  void thread_update(int ant_start_index, int ant_end_index);
  inline glm::vec2 getFoodLocation() const { return m_foodLocation; }

private:
  std::vector<Ant> m_ants;
  std::vector<std::thread> m_antThreads;
  std::vector<int> m_food;
  glm::vec2 m_foodLocation;
  CellGrid<PheromoneCell> m_pheromoneGrid;

  // --- <> Threading <> --- //
  std::mutex mtx;
  std::mutex doneMtx;
  std::condition_variable cv;
  std::condition_variable doneCv;

  std::atomic<bool> stop;
  std::atomic<std::size_t> currentGeneration;
  std::atomic<std::size_t> doneCount;
};
} // namespace al
