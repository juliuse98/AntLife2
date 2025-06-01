#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <easylogging++.h>

class Simulator {
public:
  Simulator(std::vector<int> &data)
      : data(data), stop(false), readyCount(0), doneCount(0) {
    worker1 = std::thread(&Simulator::workerThread, this, 0, data.size() / 2);
    worker2 = std::thread(&Simulator::workerThread, this, data.size() / 2,
                          data.size());
  }

  ~Simulator() {
    {
      std::lock_guard<std::mutex> lock(mtx);
      stop = true;
    }
    cv.notify_all();
    worker1.join();
    worker2.join();
  }

  void updateFrame() {
    {
      std::unique_lock<std::mutex> lock(mtx);
      readyCount = 0;
      doneCount = 0;
      workReady = true;
    }
    cv.notify_all();

    // Wait until both workers are done
    std::unique_lock<std::mutex> lock(doneMtx);
    doneCv.wait(lock, [this] { return doneCount == 2; });
  }

private:
  std::vector<int> &data;
  std::thread worker1, worker2;
  std::mutex mtx, doneMtx;
  std::condition_variable cv, doneCv;
  std::atomic<bool> stop;
  bool workReady = false;
  int readyCount = 0;
  int doneCount = 0;

  void workerThread(size_t begin, size_t end) {
    while (true) {
      // Wait for signal
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [this] { return workReady || stop; });
      if (stop)
        return;
      lock.unlock();

      // Process assigned range
      for (size_t i = begin; i < end; ++i) {
        // simulate something
        data[i] += 1;
        LOG(INFO) << "Do stuff";
      }

      // Notify main thread we're done
      {
        std::lock_guard<std::mutex> doneLock(doneMtx);
        doneCount++;
        if (doneCount == 2) {
          workReady = false;
          doneCv.notify_one();
        }
      }
    }
  }
};
