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

  _settings.color.s[0] = _avalaibleColors[_colorIdx].r;
  _settings.color.s[1] = _avalaibleColors[_colorIdx].g;
  _settings.color.s[2] = _avalaibleColors[_colorIdx].b;

  // Full opacity
  _settings.color.s[3] = 1.0f;

  if (!_camera) _camera = new Camera(Camera::initialPos, glm::vec3(0.0f), _gl);
}

void ParticleSystem::_handleInput(void) {
  glfwPollEvents();

  if (_gl.isKeyJustPressed(GLFW_KEY_ESCAPE) == true)
    glfwSetWindowShouldClose(_gl.getWindow(), GL_TRUE);
  if (_gl.isKeyJustPressed(GLFW_KEY_TAB)) _camera->toggleFreeNavMode();
  if (_gl.isKeyJustPressed(GLFW_KEY_P) ||
      (!_camera->isInFreeNavMode() && _gl.isKeyJustPressed(GLFW_KEY_SPACE)))
    _isPaused = !_isPaused;

  if (_gl.isKeyJustPressed(GLFW_KEY_G))
    _settings.gravityEnabled = !_settings.gravityEnabled;

  if (!_isPaused) {
    _resetShapeIfNeeded();
    _changeColorIfNeeded();
  }
}

void ParticleSystem::_resetShapeIfNeeded(void) {
  // Current shape reset
  if (_gl.isKeyJustPressed(GLFW_KEY_R))
    return _cl.setParticles(_numParticles, _initFuncNames[_funcIdx]);

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

void ParticleSystem::_changeColorIfNeeded(void) {
  bool up = _gl.isKeyJustPressed(GLFW_KEY_UP);
  bool down = _gl.isKeyJustPressed(GLFW_KEY_DOWN);
  if (up || down) {
    if (up) {
      if (_colorIdx == 0)
        _colorIdx = _avalaibleColors.size() - 1;
      else
        _colorIdx--;
    }
    if (down) {
      _colorIdx++;
      if (_colorIdx >= _avalaibleColors.size()) _colorIdx = 0;
    }
    _settings.color.s[0] = _avalaibleColors[_colorIdx].r;
    _settings.color.s[1] = _avalaibleColors[_colorIdx].g;
    _settings.color.s[2] = _avalaibleColors[_colorIdx].b;
  }
}

glm::vec3 ParticleSystem::_rayCastToXYPlane(glm::vec2 const &screenPos) {
  float x = (2.0f * screenPos.x) / _gl.getWidth() - 1.0f;
  float y = 1.0f - (2.0f * screenPos.y) / _gl.getHeight();

  glm::vec4 rayClipSpace(x, y, -1.0f, 1.0f);
  glm::vec4 rayEyeSpace =
      glm::inverse(_camera->getProjectionMatrix()) * rayClipSpace;
  rayEyeSpace.z = -1.0f;
  rayEyeSpace.w = 0.0f;

  glm::vec3 rayDir = (glm::inverse(_camera->getViewMatrix()) * rayEyeSpace);
  rayDir = glm::normalize(rayDir);

  glm::vec3 rayOrig = _camera->getPosition();
  glm::vec3 planeInvNorm(0.0, 0.0, -1.0);

  float length =
      -glm::dot(rayOrig, planeInvNorm) / fabs(glm::dot(planeInvNorm, rayDir));
  return (rayOrig + rayDir * length);
}

void ParticleSystem::_updateParticles(void) {
  if (!_camera->isInFreeNavMode() && _settings.gravityEnabled) {
    _gravityCenter = _rayCastToXYPlane(_gl.getMousePos());
  }

  _cl.updateParticles(_numParticles, _gravityCenter, _settings);
}

void ParticleSystem::runLoop(void) {
  double currentTime = glfwGetTime();
  _gl.lastTime = currentTime;
  _gl.lastFPSUpdateTime = currentTime;
  while (!glfwWindowShouldClose(_gl.getWindow())) {
    _handleInput();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (_camera->isInFreeNavMode()) _camera->update();

    // Update particles positions and velocities
    if (!_isPaused) _updateParticles();

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

std::vector<glm::vec3> ParticleSystem::_getAvalaibleColors(void) {
  std::vector<glm::vec3> vector;
  vector.push_back(glm::vec3(1.0f, 0.549f, 0.0f));      // Dark orange
  vector.push_back(glm::vec3(1.0f, 0.843f, 0.0f));      // Gold
  vector.push_back(glm::vec3(0.196f, 0.804f, 0.196f));  // Lime green
  vector.push_back(glm::vec3(0.392f, 0.584f, 0.929f));  // Corn flower blue
  return vector;
}

std::vector<char const *> ParticleSystem::_initFuncNames = _getInitFuncNames();

std::vector<glm::vec3> ParticleSystem::_avalaibleColors = _getAvalaibleColors();
