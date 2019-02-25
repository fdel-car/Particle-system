#pragma once

#include "CLContext.hpp"
#include "Camera.hpp"
#include "GLRenderer.hpp"

class ParticleSystem {
 public:
  ParticleSystem(GLRenderer &gl, CLContext &cl, size_t numParticles);

  virtual ~ParticleSystem(void);

  void runLoop(void);

 private:
  GLRenderer &_gl;
  CLContext &_cl;
  size_t _numParticles;
  Camera *_camera = nullptr;
  size_t _funcIdx = 0;
  size_t _colorIdx = 0;
  glm::vec3 _gravityCenter = glm::vec3(0.0f);
  Settings _settings;
  bool _isPaused = false;

  static std::vector<char const *> _initFuncNames;
  static std::vector<glm::vec3> _avalaibleColors;

  void _init(void);
  void _handleInput(void);
  void _resetShapeIfNeeded(void);
  void _changeColorIfNeeded(void);
  void _updateParticles(void);
  glm::vec3 _rayCastToXYPlane(glm::vec2 const &screenPos);

  static std::vector<char const *> _getInitFuncNames(void);
  static std::vector<glm::vec3> _getAvalaibleColors(void);

  ParticleSystem(void);
  ParticleSystem(ParticleSystem const &src);

  ParticleSystem &operator=(ParticleSystem const &rhs);
};
