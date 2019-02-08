#pragma once

#include "ParticleSystem.hpp"

#define WIDTH 1280
#define HEIGHT 720

#include <array>

class GLRenderer {
 public:
  static std::array<bool, 1024> inputs;

  GLRenderer(void);
  virtual ~GLRenderer(void);

  GLFWwindow *getWindow(void) const;

 private:
  int _width, _height;

  GLFWwindow *_window = nullptr;

  GLRenderer(GLRenderer const &src);

  static void _keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);

  GLRenderer &operator=(GLRenderer const &rhs);
};
