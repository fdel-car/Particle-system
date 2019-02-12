#include "ParticleSystem.hpp"

ParticleSystem::ParticleSystem(GLRenderer &gl, CLContext &cl,
                               size_t numParticles)
    : _gl(gl), _cl(cl), _numParticles(numParticles) {
  _init();
}

ParticleSystem::~ParticleSystem(void) {
  if (_camera) delete _camera;
}

void ParticleSystem::_init(void) {
  _cl.addSource("init.cl");
  _cl.buildProgram();

  _gl.initMemory(_numParticles);
  _cl.initMemory(_gl.VBO, _numParticles);

  _camera = new Camera(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0f), _gl);

  glEnable(GL_PROGRAM_POINT_SIZE);
  glPointSize(4.0f);
}

void ParticleSystem::runLoop(void) {
  double lastTime = 0.0;
  _gl.lastTime = glfwGetTime();
  int nbFrames = 0;
  while (!glfwWindowShouldClose(_gl.getWindow())) {
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 0.25) {
      glfwSetWindowTitle(_gl.getWindow(), std::to_string(nbFrames * 4).c_str());
      nbFrames = 0;
      lastTime += 0.25;
    }

    glfwPollEvents();
    if (_gl.isKeyJustPressed(GLFW_KEY_ESCAPE) == true)
      glfwSetWindowShouldClose(_gl.getWindow(), GL_TRUE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    _camera->update();

    _gl.displayParticles(_numParticles, _camera->getProjectionMatrix() *
                                            _camera->getViewMatrix());

    glfwSwapBuffers(_gl.getWindow());
  }
}
