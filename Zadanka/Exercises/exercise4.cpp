#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <ranges>
#include <sstream>
#include <thread>

namespace fs = std::filesystem;

std::string words[] = {"Lorem", "Ipsum", "Sit", "Dolorem"};

void createRandomFile(fs::path root, std::default_random_engine &engine) {
  size_t fileLength = std::uniform_int_distribution<size_t>(20, 30)(engine);
  std::ofstream file(root);
  for (size_t j = 0; j < fileLength; ++j) {
    file << words[std::uniform_int_distribution<size_t>(0, 3)(engine)];
  }
  file << std::endl;
}

void populateDirectory(fs::path root, std::default_random_engine &engine) {
  size_t fileCount = std::uniform_int_distribution<size_t>(0, 5)(engine);
  for (size_t i = 0; i < fileCount; ++i) {
    createRandomFile(root / ("file" + std::to_string(i + 1) + ".txt"), engine);
  }
}

void createRandomTree(fs::path root, std::default_random_engine &engine,
                      size_t level) {
  populateDirectory(root, engine);

  auto dirCount = std::uniform_int_distribution<size_t>(0, level / 2)(engine);
  for (size_t i = 0; i < dirCount; ++i) {
    std::string dirName = (root / ("dir" + std::to_string(i)));
    fs::create_directory(dirName);
    createRandomTree(dirName, engine, level - 1);
  }
}

fs::path initializeEnvironment(const std::string &folderName) {
  std::default_random_engine engine(7312);
  auto rootPath = fs::current_path() / folderName;
  fs::remove_all(rootPath);
  fs::create_directory(rootPath);
  createRandomTree(rootPath, engine, 6);
  return rootPath;
}

// Twoim zadaniem jest jak najszybsze zaszyfrowanie folderu z wrażliwymi danymi
// Za pomocą szyfru cezara(skorzystaj z std::ranges) zaszyfruj wszystkie pliki w folderze fsroot, użyj
// wielowątkowości(std::packaged_task może się przydać)
// PS. Mam nadzieje że w folderze z projektem nie macie żadnego ważnego folderu nazwanego fsroot...

void exercise4(){
  auto rootPath = initializeEnvironment("fsroot");
  //Miejsce na rozwiązanie
}

int main() {
  exercise4();
  return 0;
}
