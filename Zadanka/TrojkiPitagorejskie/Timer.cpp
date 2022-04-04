#include "./Timer.hpp"

TimePoint startTimer(){
  return std::chrono::system_clock::now();
}

double resetTimer(TimePoint point){
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - point).count();
}

Timer::Timer(int &out) : output_value(&out), startTime(startTimer()){}

Timer::~Timer() {
std::cout << "After " << resetTimer(Timer::startTime) << " milliseconds " << *Timer::output_value <<
" Pythagorean triples were found" << std::endl;
}
