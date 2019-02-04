#include "Renderer.hpp"

Renderer::Renderer(void) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  _monitor = glfwGetPrimaryMonitor();
  _mode = glfwGetVideoMode(_monitor);
  _window =
      glfwCreateWindow(WIDTH, HEIGHT, "Particle system", nullptr, nullptr);
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create windows GLFW");
  }
  glfwSetWindowPos(_window, (_mode->width / 2) - (WIDTH / 2),
                   (_mode->height / 2) - (HEIGHT / 2));
  glfwGetFramebufferSize(_window, &_width, &_height);
  glfwMakeContextCurrent(_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");
  glViewport(0, 0, _width, _height);
  glfwSetKeyCallback(_window, _keyCallback);
  // glfwSetCursorPosCallback(_window, mouseCallback);
}

Renderer::~Renderer(void) {}

std::array<bool, 1024> Renderer::inputs = std::array<bool, 1024>();

void Renderer::_keyCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_RELEASE) {
    inputs[key] = GLFW_PRESS ? true : false;
  }
  (void)scancode;
  (void)window;
  (void)mods;
}