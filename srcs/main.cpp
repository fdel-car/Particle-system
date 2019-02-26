#include <regex>
#include "ParticleSystem.hpp"

int main(int argc, char **argv) {
  size_t numParticles = 1000000;
  if (argc > 1) {
    std::regex nbrPattern("[0-9]+");
    if (!std::regex_match(argv[1], nbrPattern)) {
      std::cerr << "The given argument must be an unsigned long integer value."
                << std::endl;
      return EXIT_FAILURE;
    }
    numParticles = strtoul(argv[1], nullptr, 0);
    if (numParticles > 16777216) {
      std::cerr << "The particles count limit is arbitrarily set to 2^24, you "
                   "can go higher than that."
                << std::endl;
      return EXIT_FAILURE;
    }
    // size_t perfectCube = pow(round(cbrt(numParticles)), 3.0f);
    // if (perfectCube != numParticles)
    //   std::cout << "The closest perfect cube is: " << perfectCube << '.'
    //             << std::endl;
  }
  try {
    GLRenderer gl;
    CLContext cl(gl);
    size_t maxWorkGroupSize = cl.getMaxWorkGroupSize();
    numParticles = (numParticles / maxWorkGroupSize + 1) * maxWorkGroupSize;
    ParticleSystem ps(gl, cl, numParticles);
    ps.runLoop();
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
