#pragma once

#include "Headers.hpp"

class GLRenderer;

class Camera;

class CLContext {
 public:
  CLContext(GLRenderer const &gl);
  virtual ~CLContext(void);

  void addSource(std::string const &fileName);
  void buildProgram(void);
  void initMemory(GLuint const &VBO);
  void setParticles(size_t numParticles, char const *funcName);
  void updateParticles(size_t numParticles, Camera const &camera);

  static std::string const getErrorString(cl_int const &error);

 private:
  std::vector<cl::Platform> _platforms;
  std::vector<cl::Device> _devices;
  cl::Platform _defaultPlatform;
  cl::Device _defaultDevice;
  cl::Context _context;
  cl::CommandQueue _queue;
  cl::Program::Sources _sources;
  cl::Program _program;
  std::vector<cl::Memory> _gl_buffers;
  GLRenderer const &_gl;

  CLContext(void);
  CLContext(CLContext const &src);

  CLContext &operator=(CLContext const &rhs);

  bool _isExtensionSupported(std::string const &reqExt,
                             std::string const &allExt) const;
};
