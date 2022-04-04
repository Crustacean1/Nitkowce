#include <iostream>
#include <future>
#include <thread>
#include <tuple>
#include <math.h>
#include <chrono>
#include <vector>
#include "Timer.hpp"

#define bigNumber 100000
//int bigNumber = 100000;

int genPitTrip(int step,int offset){
  int findings = 0;
  for(int i = offset;findings< (bigNumber + step - 1)/step;i += step){
    for(int j = 1;j<i;++j){
      int potentialSquare = i * i + j * j;
      int root = sqrt(potentialSquare);
      if(root * root == potentialSquare){
        //std::cout<<i<<" "<<j<<" "<<potentialSquare<<std::endl;
        ++findings;
        //return std::make_tuple(i,j);
      }
    }
  } 
  std::cout<<"Found: "<<findings<<"triplets"<<std::endl;
  return findings;
}



int main(int argc,char ** argv){
  std::cout<<""<<std::endl;
  auto pitTriple = std::async(std::launch::async,genPitTrip,1,1);
  {
    int normalSum;
    Timer timer(normalSum);
  //  auto startTime = startTimer();
    normalSum = pitTriple.get();
//    std::cout << "after: " << resetTimer(startTime) << " milliseconds found: " << normalSum << " triplets" << std::endl;
  }

  int cores = std::thread::hardware_concurrency();
  std::cout<<"System has: "<<cores<<" cores"<<std::endl;

  std::vector<std::future<int>> futures;
  for(int i = 0;i<cores;++i){
    futures.push_back(std::async(std::launch::async,genPitTrip,cores,i+1));
  }
  {
    int threadSum = 0;
    Timer timer(threadSum);
    for (int i = 0; i < cores; ++i) {
      threadSum += futures[i].get();
    }
 //   std::cout << "after: " << resetTimer(startTime) << " milliseconds found: " << threadSum << " triplets" << std::endl;
  }

}
  
