#include "Timer.hpp"
#include <chrono>
#include <future>
#include <iostream>
#include <math.h>
#include <thread>
#include <tuple>
#include <vector>

int bigNumber = 10;

struct Triple {
  int a;
  int b;
  int c;
};

std::vector<Triple> genPitTrip(int step, int offset) {
  int findings = 0;
  std::vector<Triple> result;
  for (int i = offset; findings < (bigNumber + step - 1) / step; i += step) {
    for (int j = 1; j < i; ++j) {
      int potentialSquare = i * i + j * j;
      int root = sqrt(potentialSquare);
      if (root * root == potentialSquare) {
        ++findings;
        result.push_back({i * i, j * j, root * root});
      }
    }
  }
  return result;
  ;
}

int main(int argc, char **argv) {
  std::cout << "" << std::endl;

  int cores = std::thread::hardware_concurrency();
  std::cout << "System has: " << cores << " cores" << std::endl;

  std::vector<std::future<std::vector<Triple>>> futures;
  for (int i = 0; i < 2; ++i) {
    futures.push_back(std::async(std::launch::async, genPitTrip, 2, i + 1));
  }
  for (int i = 0; i < futures.size(); ++i) {
    auto triples = futures[i].get();
    for (int j = 0; j < triples.size(); ++j) {
      std::cout << "Triplet: " << triples[j].a << "\t" << triples[j].b << "\t"
                << triples[j].c << std::endl;
    }
    std::cout<<"Sep"<<std::endl;
  }
  //   std::cout << "after: " << resetTimer(startTime) << " milliseconds
  //   found: " << threadSum << " triplets" << std::endl;
}
