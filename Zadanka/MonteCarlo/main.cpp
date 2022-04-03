#include <chrono>
#include <future>
#include <iostream>
#include <random>

double monteCarloPi(size_t seed, size_t iterationCount) {
  std::default_random_engine engine(std::chrono::system_clock().now().time_since_epoch().count() + seed);
  size_t successCount = 0;
  std::uniform_real_distribution<double> dist(0, 1);
  double x, y;

  for (size_t i = 0; i < iterationCount; ++i) {
    x = dist(engine);
    y = dist(engine);
    if (x * x + y * y <= 1) {
      ++successCount;
    }
  }
  return ((double)(successCount << 2) / (double)iterationCount);
}

double parallelMonteCarlo(size_t threadCount, size_t iterationCount) {
  std::vector<std::future<double>> partialResults;
  std::vector<std::thread> threads;

  for (size_t i = 0; i < threadCount; ++i) {
    std::packaged_task<double(size_t,size_t)> task(monteCarloPi);
    partialResults.push_back(task.get_future());
    threads.emplace_back(std::thread(std::move(task),i, iterationCount));
  }
  double result = 0;
  for (auto &res : partialResults) {
    auto partial = res.get();
    std::cout<<"Partial result: "<<partial<<std::endl;
    result += partial;
  }
  for (auto &thread : threads) {
    thread.join();
  }
  return result / (double)threadCount;
}

// Oblicz pi w 100 iteracjach algorytmu monte carlo następnie porównaj
// dokładność wersji jednowątkowej z wielowątkową, wykorzystaj std::packaged_task
//

int main(int argc, char **argv) {
  std::cout.precision(24);
  std::cout << monteCarloPi(0,100) << std::endl;
  std::cout << parallelMonteCarlo(std::thread::hardware_concurrency(), 100)
            << std::endl;
  return 0;
}
