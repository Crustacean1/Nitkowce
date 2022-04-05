#include <future>
#include <iostream>
#include <math.h>
#include <regex>
#include <thread>
#include <vector>

struct PolyTerm {
  double coeff;
  double power;
};

double computePolynomial(const std::vector<PolyTerm> &terms, double x) {
  double result = 0;
  for (const auto &term : terms) {
    result += term.coeff * pow(x, term.power);
  }
  return result;
}

PolyTerm toTerm(const std::string &coefficient, const std::string &power) {
  return PolyTerm{
      (double)(coefficient.size() == 0 ? 1 : std::atoi(coefficient.c_str())),
      (double)(power.size() == 0 ? 1 : std::atoi(power.c_str())),
  };
}

// Otrzymasz string z wielomianem postaci (ax^b + cx^d + ...) z całkowitymi
// współczynnikami i potęgami oraz bez stałych. Należy sparsować go za pomocą
// wyrażeń typu regex do postaci std::vector<PolyTerm>. Następnie z użyciem
// funkcji computePolynomial zcałkuj go numerycznie na przedziale (-5, 5)
// W rozwiązaniu wykorzystaj wielowątkowść, do tego celu użyj std::promise

void exercise3(){
  //Miejsce na rozwiązanie
}

int main(int argc, char **argv) {
  exercise3();
  return 0;
}
