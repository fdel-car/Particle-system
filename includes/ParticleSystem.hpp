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

  static std::vector<char const *> _initFuncNames;

  void _init(void);
  void _resetPatternIfNeeded(void);

  static std::vector<char const *> _getInitFuncNames(void);

  ParticleSystem(void);
  ParticleSystem(ParticleSystem const &src);

  ParticleSystem &operator=(ParticleSystem const &rhs);
};
