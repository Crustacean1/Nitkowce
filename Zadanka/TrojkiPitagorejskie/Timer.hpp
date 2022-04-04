#ifndef NITKOWCE_ZADANKA_TIMER_TIMER_HPP
#define NITKOWCE_ZADANKA_TIMER_TIMER_HPP
#include <chrono>
#include <iostream>

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
class Timer {
  TimePoint startTime;
  int *output_value;
public:
  void operator()();
  Timer(int &out);
  ~Timer();
};

#endif //NITKOWCE_ZADANKA_TIMER_TIMER_HPP
