#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <math.h>
#include <random>
#include <regex>
#include <sstream>

namespace fs = std::filesystem;

double dist(double x, double y) { return sqrt(pow(x, 2) + pow(y, 2)); }

double epsilon = 0.1;

std::string generateTrianglePoints(std::default_random_engine &engine,
                                   bool impostor) {
  size_t minPointDistance =
      2; // Ensures sufficient precision for angle to be 'quite' right

  std::uniform_real_distribution<double> posDist(-21, 21);
  double posX = posDist(engine);
  double posY = posDist(engine);

  double posX2 = posDist(engine);
  double posY2 = posDist(engine);

  while (dist(posX - posX2, posY - posY2) < minPointDistance) {
    posX2 = posDist(engine);
    posY2 = posDist(engine);
  }

  std::uniform_real_distribution<double> lengthDist(1, 2);
  double side2 = lengthDist(engine);

  double posX3 = posX2 + side2 * (posY - posY2);
  double posY3 = posY2 + side2 * (posX2 - posX);

  if (impostor) {
    posX3 += 10; // what are the odds...
  }

  std::stringstream ss;
  ss << "triangle["
     << "(" << posX << "," << posY << "),";
  ss << ""
     << "(" << posX2 << "," << posY2 << "),";
  ss << ""
     << "(" << posX3 << "," << posY3 << ")]";
  return ss.str();
}

void fillFile(fs::path root, std::default_random_engine &engine, int fileSize,
              bool isSus) {

  int susLine = -1; // Skip by default
  std::ofstream file(root);

  if (isSus) {
    susLine = std::uniform_int_distribution<int>(0, fileSize - 1)(engine);
  }

  for (int j = 0; j < fileSize; ++j) {
    if (j == susLine) {
      std::cout << "Impostor: " << root << " \t file: " << root
                << "\tline : " << j
                << std::endl; // Helps to verify solution correctness
    }
    file << generateTrianglePoints(engine, j == susLine) << "\n";
  }
}

void fillDirectoryWithFiles(fs::path root, int count, int triangleCount,
                            std::default_random_engine &engine,
                            int impostorFile) {
  for (int i = 0; i < count; ++i) {
    std::string filename = root / ("file" + std::to_string(i + 1) + ".txt");
    fillFile(root / filename, engine, 1000, i == impostorFile);
  }
}

void createRandomTree(fs::path root, std::default_random_engine &engine,
                      size_t level, bool evil) {
  int trianglesInFile = 10;
  int filesInDir = 3;
  int childDir = level;
  int evilDir =
      evil ? std::uniform_int_distribution<int>(-1, childDir - 1)(engine) : -1;
  int evilFile =
      (evil && evilDir == -1)
          ? std::uniform_int_distribution<int>(0, filesInDir - 1)(engine)
          : -1;

  fillDirectoryWithFiles(root, filesInDir, trianglesInFile, engine, evilFile);

  for (int i = 0; i < childDir; ++i) {
    std::string dirName = (root / ("dir" + std::to_string(i)));
    fs::create_directory(dirName);
    createRandomTree(dirName, engine, level - 1, i == evilDir);
  }
}

void createRoot() {
  std::default_random_engine engine(471576);
  auto rootPath = fs::current_path() / "fsroot";
  fs::remove_all(rootPath);
  fs::create_directory(rootPath);
  createRandomTree(rootPath, engine, 3, true);
}

// Solution

double sqrDist(double x, double y) { return pow(x, 2) + pow(y, 2); }

bool isTriangleValid(double x1, double y1, double x2, double y2, double x3,
                     double y3) {
  auto a = sqrDist(x1 - x2, y1 - y2);
  auto b = sqrDist(x2 - x3, y2 - y3);
  auto c = sqrDist(x3 - x1, y3 - y1);

  bool valid = abs((a + b) - c) < epsilon || abs((a + c) - b) < epsilon ||
               abs((c + b) - a) < epsilon;
  return valid;
}

bool isTriangleValid(std::smatch match) { // Clunky...
  return isTriangleValid(std::atof(std::string(match[1]).c_str()),
                         std::atof(std::string(match[2]).c_str()),
                         std::atof(std::string(match[3]).c_str()),
                         std::atof(std::string(match[4]).c_str()),
                         std::atof(std::string(match[5]).c_str()),
                         std::atof(std::string(match[6]).c_str()));
}

std::string isValidFile(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  std::string number = "(-?[0-9]+\\.-?[0-9]+)";
  std::string numberPair = "\\(" + number + "," + number + "\\)";
  std::regex triangle("triangle\\[" + numberPair + "," + numberPair + "," +
                      numberPair + "\\]");
  std::smatch match;

  size_t linePos = 0;
  while (file) {
    getline(file, line);
    if (!file) {
      break;
    }
    if (std::regex_match(line, match, triangle)) {
      if (!isTriangleValid(match)) {
        return filename + " at: " + std::to_string(linePos);
      }
    }
    ++linePos;
  }
  return "";
}

void findImpostor(fs::path root) {
  std::vector<std::future<std::string>> validity;
  for (auto &file : fs::recursive_directory_iterator(root)) {
    if (fs::is_regular_file(file)) {
      std::future<std::string> fut = std::async(
          std::launch::async, &isValidFile, std::string(file.path()));
      validity.emplace_back(std::move(fut));
    }
  }
  for (std::future<std::string> &file : validity) {
    std::string result = file.get();
    if (result != "") {
      std::cout << "Sus file: " << result << std::endl;
    }
  }
}

// Zadanie trójkąty
// Przeszukaj rekurencyjnie wygenerowany folder, w środku znajdują się pliki,
// w każdym z nich znajduje się kilka linijek postaci triangle[(,),(,),(,)]
// w sposób współbieżny znajdż plik ze skorumpowanym trójkątem -> takim który
// nie jest prosty

int main(int argc, char **argv) {
  createRoot();
  auto root = fs::current_path() / "fsroot";
  findImpostor(root);
  return 0;
}
