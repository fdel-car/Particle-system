#pragma once

#include "ParticleSystem.hpp"

class CLProgram {
 public:
  CLProgram(void);
  virtual ~CLProgram(void);

 private:
  cl::Platform _defaultPlatform;
  cl::Device _defaultDevice;
  cl::Context _context;
  cl::CommandQueue _queue;

  CLProgram(CLProgram const &src);

  CLProgram &operator=(CLProgram const &rhs);

  bool _isExtensionSupported(std::string const &reqExt,
                             std::string const &allExt) const;
};
