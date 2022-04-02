#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

namespace fs = std::filesystem;

std::string words[] = {"Lorem", "Ipsum", "Sit", "Dolorem"};

void createRandomTree(fs::path root, std::default_random_engine &engine,
                      size_t level) {
  auto fileCount = std::uniform_int_distribution<size_t>(0, 5)(engine);

  for (size_t i = 0; i < fileCount; ++i) {
    auto textLength = std::uniform_int_distribution<size_t>(5, 20)(engine);
    std::string filename = root / ("file" + std::to_string(i + 1) + ".txt");
    std::ofstream file(filename);
    for (size_t j = 0; j < textLength; ++j) {
      file << words[std::uniform_int_distribution<size_t>(0, 3)(engine)];
    }
    file<<std::endl;
  }

  auto dirCount = std::uniform_int_distribution<size_t>(0, level)(engine);
  for (size_t i = 0; i < dirCount; ++i) {
    std::string dirName = (root / ("dir" + std::to_string(i)));
    fs::create_directory(dirName);
    createRandomTree(dirName, engine, level - 1);
  }
}

void ceasars(std::istream input, std::ofstream output){
  std::string buffer;
  while(file>>buffer){
    
  }
}
void encryptDirectory(){
  
}

int main() {
  std::default_random_engine engine(7312);

  fs::remove_all(fs::current_path() / "fsroot");
  fs::create_directory(fs::current_path() / "fsroot");
  createRandomTree(fs::current_path() / "fsroot", engine, 3);

  return 0;
}
