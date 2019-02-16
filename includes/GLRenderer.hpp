#pragma once

#include "ShaderProgram.hpp"

#define WIDTH 1280
#define HEIGHT 720

#include <map>

struct KeyState {
  bool currFrame;
  bool prevFrame;
};

class GLRenderer {
 public:
  GLRenderer(void);

  GLuint VAO, VBO;
  double deltaTime, currentTime, lastTime;
  double lastFPSUpdateTime;

  virtual ~GLRenderer(void);

  void updateMousePos(void) const;
  void initMemory(size_t numParticles);
  bool isKeyPressed(int keyID) const;
  bool isKeyJustPressed(int keyID) const;
  void displayParticles(size_t numParticles, glm::mat4 const &VP);
  void switchCursorMode(bool debugMode) const;

  GLFWwindow *getWindow(void) const;
  glm::vec2 const getMousePos(void) const;
  int getWidth(void) const;
  int getHeight(void) const;

 private:
  int _nbFrames = 0;
  int _width, _height;
  GLFWwindow *_window = nullptr;
  ShaderProgram *_shaderProgram = nullptr;

  static glm::vec2 _mousePos;
  static std::map<int, KeyState> _keyboardMap;

  GLRenderer(GLRenderer const &src);

  void _centerWindow(GLFWmonitor *monitor);
  void _initShader(void);

  static void _keyStateUpdate(int keyID, bool isPressed);
  static void _keyCallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void _cursorCallback(GLFWwindow *window, double xpos, double ypos);

  GLRenderer &operator=(GLRenderer const &rhs);
};
