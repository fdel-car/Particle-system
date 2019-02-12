#include "ParticleSystem.hpp"

int main() {
  size_t numParticles = 1024;
  try {
    GLRenderer gl;
    CLContext cl(gl);
    ParticleSystem ps(gl, cl, numParticles);
    ps.runLoop();
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
