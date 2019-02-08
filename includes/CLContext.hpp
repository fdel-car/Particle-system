#pragma once

#include "ParticleSystem.hpp"

class CLContext {
 public:
  CLContext(void);
  virtual ~CLContext(void);

  void addSource(std::string const &src);
  void buildProgram(void);
  void initMemory(GLuint const &VBO);
  cl::Context &getContext(void);

  static std::string const getErrorString(cl_int const &error);

  cl::Platform _defaultPlatform;
  cl::Device _defaultDevice;
  cl::Context _context;
  cl::CommandQueue _queue;
  cl::Program::Sources _sources;
  cl::Program _program;
  std::vector<cl::Memory> _gl_buffers;

 private:
  std::vector<cl::Device> _devices;

  CLContext(CLContext const &src);

  CLContext &operator=(CLContext const &rhs);

  bool _isExtensionSupported(std::string const &reqExt,
                             std::string const &allExt) const;
};
