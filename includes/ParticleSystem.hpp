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

  void _init(void);

  ParticleSystem(void);
  ParticleSystem(ParticleSystem const &src);

  ParticleSystem &operator=(ParticleSystem const &rhs);
};
