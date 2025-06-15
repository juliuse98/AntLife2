#include "world.h"
#include "ant.h"

namespace al {
World::World(int antCount)
    : stop(false), currentGeneration(0), doneCount(0),
      m_pheromoneGrid(1000.0f, 6), m_foodLocation(0.0f, 3.01f),
      m_size(300, 300) {
  m_ants.reserve(antCount);

  for (int i = 0; i < antCount; ++i) {
    float heading = 360.0f * i / antCount;
    m_ants.emplace_back(m_size.x / 2, m_size.y / 2, heading);
  }
  for (int tc = 0; tc < static_cast<int>(std::ceil(
                            static_cast<double>(antCount) / ANTS_PER_THREAD));
       ++tc) {
    const int start = tc * ANTS_PER_THREAD;
    const int end = tc * ANTS_PER_THREAD + ANTS_PER_THREAD - 1;
    LOG(INFO) << "Create thread with start at " << start << " and end at "
              << end;
    m_antThreads.push_back(
        std::thread(&World::thread_update, this, tc, start, end));
  }
}

World::~World() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop = true;
  }
  cv.notify_all();
  for (auto &thread : m_antThreads) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void World::update() {
  std::size_t generationForThisUpdate;
  {
    std::unique_lock<std::mutex> lock(mtx);
    currentGeneration++;
    generationForThisUpdate = currentGeneration;
    doneCount = 0;
    LOG(INFO) << "Main: Starting update for generation "
              << generationForThisUpdate;
  }
  cv.notify_all();

  std::unique_lock<std::mutex> lock_done(doneMtx);
  doneCv.wait(lock_done, [this, generationForThisUpdate] {
    return doneCount == m_antThreads.size();
  });
  for (int i = 0; i < m_ants.size() && i < 1; ++i) {

    Ant &ant = m_ants.at(i);

    LOG(INFO) << " New ant" << i << " pos: " << ant.position.x << ", "
              << ant.position.y;
  }
  LOG(INFO) << "Main: All threads done with generation "
            << generationForThisUpdate << ". Final doneCount: " << doneCount;
}

void World::thread_update(int thread_index, int ant_start_index,
                          int ant_end_index) {
  std::size_t lastProcessedGeneration = 0;

  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this, &lastProcessedGeneration] {
      return (currentGeneration > lastProcessedGeneration) || stop;
    });

    if (stop) {
      LOG(INFO) << "Thread " << std::this_thread::get_id() << " stopping.";
      return;
    }

    std::size_t generationToProcess = currentGeneration;
    lock.unlock();

    LOG(INFO) << "Thread " << std::this_thread::get_id() << " (ants "
              << ant_start_index << "-" << ant_end_index
              << ") starting work for generation " << generationToProcess;

    const int endIndex =
        (ant_end_index >= m_ants.size()) ? m_ants.size() - 1 : ant_end_index;

    LOG(INFO) << "Thread " << std::this_thread::get_id() << " processing ants "
              << ant_start_index << " to " << endIndex;
    for (int i = ant_start_index; i <= endIndex; ++i) {
      if (i < m_ants.size()) {
        m_ants.at(i).update(1, *this);
      }
    }

    std::lock_guard<std::mutex> doneLock(doneMtx);
    doneCount++;
    LOG(INFO) << "Thread " << std::this_thread::get_id()
              << " finished generation " << generationToProcess
              << ", doneCount now " << doneCount;

    if (doneCount == m_antThreads.size()) {
      LOG(INFO) << "Thread " << std::this_thread::get_id()
                << " is the last one for generation " << generationToProcess
                << ", notifying main thread.";
      doneCv.notify_one();
    }
    lastProcessedGeneration = generationToProcess;
  }
}
} // namespace al
