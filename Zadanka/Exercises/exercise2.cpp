#include <iostream>
#include <future>
#include <thread>
#include <tuple>
#include <math.h>
#include <chrono>
#include <vector>

int tripletCount = 100000; // Dostosuj do możliwości swojej maszyny...

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
TimePoint startTimer(){
  return std::chrono::system_clock::now();
}

double resetTimer(TimePoint point){
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - point).count();
}

// Znajdż n kolejnych trójek pitagorejskich
// Użyj w tym celu wielowątkwości, porównaj wpływ liczby wątków
// roboczych na czas działania programu, oraz jak ma się on do liczby fizycznych
// wątków w systemie

void exercise2(){

}

int main(int argc,char ** argv){
  exercise2();
}
