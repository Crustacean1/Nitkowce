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
    file << std::endl;
  }

  auto dirCount = std::uniform_int_distribution<size_t>(0, level / 2)(engine);
  for (size_t i = 0; i < dirCount; ++i) {
    std::string dirName = (root / ("dir" + std::to_string(i)));
    fs::create_directory(dirName);
    createRandomTree(dirName, engine, level - 1);
  }
}

// Solution

using EncryptFunction = std::string (*)(const std::string &data,
                                        unsigned char key);

void encrypt(const std::string &filename, EncryptFunction func,
             unsigned char key) {
  std::ifstream input(filename);
  if (!input) {
    return;
  }
  std::string content, buffer;
  while (input >> buffer) {
    content += buffer;
  }
  auto encrypted = func(content, key);
  input.close();
  std::ofstream output(filename);
  if (!output) {
    return;
  }
  output << encrypted;
}

std::string ceasars(const std::string &buffer, unsigned char key) {
  auto encoded =
      buffer | std::views::transform([key](char chr) { return chr + key; });
  return std::string(encoded.begin(), encoded.end());
}

void encryptDirectory(fs::path root, unsigned char key) {
  std::vector<std::future<void>> encryptionFutures;

  for (auto &dirIt : fs::recursive_directory_iterator(root)) {
    if (dirIt.is_regular_file()) {
      std::string filename = dirIt.path();
      std::cout << "Found regular file at: " << filename << std::endl;
      std::packaged_task<void(std::string, EncryptFunction, unsigned char)>
          task(encrypt);
      encryptionFutures.push_back(task.get_future());
      task(filename, ceasars, key);
    }
  }
  for (auto &enc : encryptionFutures) {
    enc.get();
  }
}

// Twoim zadaniem jest jak najszybsze zaszyfrowanie folderu z wrażliwymi danymi
// Za pomocą szyfru cezara zaszyfruj wszystkie pliki w folderze fsroot, użyj wielowątkowości

int main() {
  std::default_random_engine engine(7312);

  auto rootPath = fs::current_path() / "fsroot";
  fs::remove_all(rootPath);
  fs::create_directory(rootPath);
  createRandomTree(rootPath, engine, 6);
  encryptDirectory(rootPath, 25);
  //encryptDirectory(rootPath, 231);

  return 0;
}
