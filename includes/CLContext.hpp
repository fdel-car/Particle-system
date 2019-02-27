#pragma once

#include "Headers.hpp"

class GLRenderer;

class CLContext {
 public:
  CLContext(GLRenderer const &gl);
  virtual ~CLContext(void);

  void addSource(std::string const &fileName);
  void buildProgram(void);
  void initMemory(GLuint const &VBO, size_t numParticles);
  void setParticles(size_t numParticles, char const *funcName);
  void updateParticles(size_t numParticles, glm::vec3 const gravityCenter,
                       cl_uchar const gravityEnabled);
  size_t getMaxWorkGroupSize(void) const;

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
  std::vector<cl::Memory> _gpu_buffers;
  GLRenderer const &_gl;
  cl::Kernel *_updateKernel = nullptr;
  size_t _maxWorkGroupSize;

  CLContext(void);
  CLContext(CLContext const &src);

  CLContext &operator=(CLContext const &rhs);

  bool _isExtensionSupported(std::string const &reqExt,
                             std::string const &allExt) const;
};
