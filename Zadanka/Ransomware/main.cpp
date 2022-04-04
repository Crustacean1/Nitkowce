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
// Za pomocą szyfru cezara(skorzystaj z std::ranges) zaszyfruj wszystkie pliki w folderze fsroot, użyj
// wielowątkowości(std::packaged_task może się przydać)
// PS. Mam nadzieje że w folderze z projektem nie macie żadnego ważnego folderu nazwanego fsroot...

int main() {
  auto rootPath = initializeEnvironment("fsroot");
  encryptDirectory(rootPath, 25);
  // encryptDirectory(rootPath, 231); // Reversing 'encryption'

  return 0;
}
