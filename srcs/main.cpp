#include "Renderer.hpp"

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
  Renderer renderer;

  bool windowShouldClose = false;
  while (!windowShouldClose) {
    glfwPollEvents();
  }

  return EXIT_SUCCESS;
}
