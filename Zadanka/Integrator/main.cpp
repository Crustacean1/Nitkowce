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

//Solution

// Assuming integer coefficients

std::vector<PolyTerm> extractTerms(const std::string &str) {
  std::vector<PolyTerm> result;
  std::string baseTermExp = "(-?[0-9]*)x(\\^(-?[0-9]*))?";
  std::regex termReg("\\+" + baseTermExp);

  std::sregex_token_iterator iterator(str.begin(), str.end(), termReg, {1, 3});

  for (; iterator != std::sregex_token_iterator{};) {
    std::string coeff = *iterator++;
    std::string power = *iterator++;
    result.push_back(toTerm(coeff, power));
  }

  std::regex firstReg(baseTermExp + "\\+.*");
  std::smatch firstTerm;

  if (std::regex_match(str, firstTerm, firstReg)) {
    result.push_back(toTerm(firstTerm[1], firstTerm[3]));
  } else {
    throw std::runtime_error("Invalid expression");
  }

  return result;
}

void intIntegrate(const std::vector<PolyTerm> &polynomial, double start,
                  double end, double step, std::promise<double> promise) {
  double result = 0;
  for (double pos = start; pos < end; pos += step) {
    result += computePolynomial(polynomial, pos) * step;
  }
  promise.set_value(result);
  return;
}

double integrate(const std::vector<PolyTerm> &polynomial, size_t noOfThreads,
                 double start, double end, double step) {
  std::vector<std::thread> threads;
  std::vector<std::future<double>> intervals;

  double bigStep = (end - start) / (double)noOfThreads;
  double result = 0;

  for (size_t i = 0; i < noOfThreads; ++i) {
    std::promise<double> intervalPromise;

    intervals.push_back(intervalPromise.get_future());

    threads.emplace_back(std::thread(
        intIntegrate, polynomial, start + i * bigStep,
        start + (i + 1) * bigStep, step, std::move(intervalPromise)));
  }
  for (auto &interval : intervals) {
    result += interval.get();
  }
  for (auto &thread : threads) {
    thread.join();
  }
  return result;
}

// Otrzymasz string z wielomianem postaci ax^b + cx^d + ...
// Twoim zadaniem jest jego numeryczne zcałkowanie z użyciem współbieżności
// do tego celu użyj std::promise

int main(int argc, char **argv) {
  auto terms = extractTerms("12x+8x");
  for (const auto &term : terms) {
    std::cout << term.coeff << "y ^ " << term.power << " + ";
  }
  std::cout << std::endl;

  std::cout << "Integral: "
            << integrate(terms, std::thread::hardware_concurrency(), -3, 3,
                         0.000001);
  return 0;
}
