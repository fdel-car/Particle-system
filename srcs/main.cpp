#include "ParticleSystem.hpp"

int main() {
  // std::vector<cl::Platform> platforms;
  // cl::Platform::get(&platforms);
  // std::vector<cl::Device> devices;
  // cl::Platform default_platform = platforms[0];
  // default_platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
  // for (auto device : devices) {
  //   std::cout << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
  //   std::cout << device.getInfo<CL_DEVICE_VERSION>() << std::endl;
  // }
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

  GLFWmonitor *_monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *_mode = glfwGetVideoMode(_monitor);

  GLFWwindow *_window =
      glfwCreateWindow(1280, 720, "Particle system", nullptr, nullptr);
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create windows GLFW");
  }
  glfwSetWindowPos(_window, (_mode->width / 2), (_mode->height / 2));
  int _width, _height;
  glfwGetFramebufferSize(_window, &_width, &_height);
  glfwMakeContextCurrent(_window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");
  glViewport(0, 0, _width, _height);
  // glfwSetKeyCallback(_window, keyCallback);
  // glfwSetCursorPosCallback(_window, mouseCallback);

  return EXIT_SUCCESS;
}
