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
  _cl.addSource("update.cl");
  _cl.buildProgram();

  _gl.initMemory(_numParticles);
  _cl.initMemory(_gl.VBO);
  _funcIdx = 0;
  _cl.setParticles(_numParticles, _initFuncNames[_funcIdx]);

  _camera = new Camera(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0f), _gl);

  // glEnable(GL_PROGRAM_POINT_SIZE);
  // glPointSize(5.0f);
}

void ParticleSystem::_resetPatternIfNeeded(void) {
  bool left = _gl.isKeyJustPressed(GLFW_KEY_LEFT);
  bool right = _gl.isKeyJustPressed(GLFW_KEY_RIGHT);

  if (left || right) {
    if (left) {
      if (_funcIdx == 0)
        _funcIdx = _initFuncNames.size() - 1;
      else
        _funcIdx--;
    }
    if (right) {
      _funcIdx++;
      if (_funcIdx >= _initFuncNames.size()) _funcIdx = 0;
    }
    _cl.setParticles(_numParticles, _initFuncNames[_funcIdx]);
  }
}

void ParticleSystem::runLoop(void) {
  double currentTime = glfwGetTime();
  _gl.lastTime = currentTime;
  _gl.lastFPSUpdateTime = currentTime;
  while (!glfwWindowShouldClose(_gl.getWindow())) {
    glfwPollEvents();
    if (_gl.isKeyJustPressed(GLFW_KEY_ESCAPE) == true)
      glfwSetWindowShouldClose(_gl.getWindow(), GL_TRUE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    _camera->update();
    // glm::mat4 VP = _camera->getProjectionMatrix() * _camera->getViewMatrix();
    if (!_camera->isInDebugMode()) {
      _resetPatternIfNeeded();
      // Update particles positions and velocities
      _cl.updateParticles(_numParticles, *_camera);
    }
    _gl.displayParticles(_numParticles, _camera->getProjectionMatrix() *
                                            _camera->getViewMatrix());
    glfwSwapBuffers(_gl.getWindow());
  }
}

std::vector<char const *> ParticleSystem::_getInitFuncNames(void) {
  std::vector<char const *> vector;
  vector.push_back("initLatitudes");
  vector.push_back("initLongitudes");
  vector.push_back("initFilledCube");
  return vector;
}

std::vector<char const *> ParticleSystem::_initFuncNames = _getInitFuncNames();
