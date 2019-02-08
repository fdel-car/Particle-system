#pragma once

#include <iostream>
#include <string>

class Tools {
 public:
  Tools(void);
  virtual ~Tools(void);

  static std::string const &readFile(std::string const &filePath);

 private:
  Tools(Tools const &src);

  Tools &operator=(Tools const &rhs);
};
