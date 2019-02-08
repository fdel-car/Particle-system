#include "Tools.hpp"

Tools::Tools(void) {}

Tools::~Tools(void) {}

std::string const Tools::readFile(std::string const &filePath) {
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  if (file) {
    std::string str;
    file.seekg(0, std::ios::end);
    str.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&str[0], str.size());
    file.close();
    return str;
  }
  throw std::runtime_error("File not found: '" + filePath + "'.");
}