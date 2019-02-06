#pragma once

#include "ParticleSystem.hpp"

#define WIDTH 1280
#define HEIGHT 720

#include <array>

class Renderer {
 public:
  static std::array<bool, 1024> inputs;

  Renderer(void);
  virtual ~Renderer(void);

  GLFWwindow *getWindow(void) const;

 private:
  int _width, _height;

  GLFWwindow *_window = nullptr;
  GLFWmonitor *_monitor = nullptr;
  const GLFWvidmode *_mode = nullptr;

  Renderer(Renderer const &src);

  static void _keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);

  Renderer &operator=(Renderer const &rhs);
};
