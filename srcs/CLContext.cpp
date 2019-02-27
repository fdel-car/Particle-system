#include "CLContext.hpp"
#include "Camera.hpp"
#include "GLRenderer.hpp"

CLContext::CLContext(GLRenderer const &gl) : _gl(gl) {
  cl::Platform::get(&_platforms);
  _defaultPlatform = _platforms.front();
  _defaultPlatform.getDevices(CL_DEVICE_TYPE_GPU, &_devices);
  _defaultDevice = _devices.front();
  _maxWorkGroupSize = _defaultDevice.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

#if defined(__APPLE__) || defined(MACOSX)
  static std::string const CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
  static std::string const CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif
  std::string const allExt = _defaultDevice.getInfo<CL_DEVICE_EXTENSIONS>();
  if (!_isExtensionSupported(CL_GL_SHARING_EXT, allExt)) {
    throw std::runtime_error("OpenGL / OpenCL interoperability not supported.");
  }

#if defined(_WIN32)
  // Windows
  cl_context_properties properties[] = {
      CL_GL_CONTEXT_KHR,
      (cl_context_properties)glfwGetWGLContext(_gl.getWindow()),
      CL_WGL_HDC_KHR,
      (cl_context_properties)GetDC(glfwGetWin32Window(_gl.getWindow())),
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)_defaultPlatform(),
      0};
#elif defined(__APPLE__) || defined(MACOSX)
  // OS X
  CGLContextObj kCGLContext = CGLGetCurrentContext();
  CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
  cl_context_properties properties[] = {
      CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
      (cl_context_properties)kCGLShareGroup, 0};
#else
  // Linux
  cl_context_properties properties[] = {
      CL_GL_CONTEXT_KHR,
      (cl_context_properties)glfwGetGLXContext(_gl.getWindow()),
      CL_GLX_DISPLAY_KHR,
      (cl_context_properties)glfwGetX11Display(),
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)_defaultPlatform(),
      0};
#endif

  cl_int err = 0;
  _context = cl::Context(_defaultDevice, properties, nullptr, nullptr, &err);
  if (err != 0)
    throw std::runtime_error("The OpenCL context initialization failed (" +
                             CLContext::getErrorString(err) + ").");
  _queue = cl::CommandQueue(_context, _defaultDevice, 0, &err);
  if (err != 0)
    throw std::runtime_error(
        "The OpenCL command queue initialization failed (" +
        CLContext::getErrorString(err) + ").");
}

CLContext::~CLContext(void) {
  if (_updateKernel) delete _updateKernel;
}

size_t CLContext::getMaxWorkGroupSize(void) const { return _maxWorkGroupSize; }

void CLContext::addSource(std::string const &fileName) {
  FILE *fp = fopen(("./srcs/kernels/" + fileName).c_str(), "rb");

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);
  char *src = (char *)malloc(size + 1);  // Possible leak here
  src[size] = '\0';
  fread(src, sizeof(char), size, fp);
  fclose(fp);

  _sources.push_back(std::make_pair(src, size));
}

void CLContext::buildProgram(void) {
  cl_int err = 0;
  _program = cl::Program(_context, _sources, &err);
  if (err != 0)
    throw std::runtime_error("The OpenCL program initialization failed (" +
                             CLContext::getErrorString(err) + ").");
  err = _program.build(_devices);
  if (err != 0)
    throw std::runtime_error("The building of the OpenCL program failed (" +
                             CLContext::getErrorString(err) + ").");
}

void CLContext::initMemory(GLuint const &VBO, size_t numParticles) {
  cl_int err = 0;
  _gpu_buffers.push_back(cl::BufferGL(
      _context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_NO_ACCESS, VBO, &err));
  if (err != 0)
    throw std::runtime_error(
        "Init CL buffer from GL buffer for positions failed (" +
        CLContext::getErrorString(err) + ").");
  _gpu_buffers.push_back(
      cl::Buffer(_context, CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS,
                 sizeof(cl_float4) * numParticles, nullptr, &err));
  if (err != 0)
    throw std::runtime_error("Init CL buffer for velocities failed (" +
                             CLContext::getErrorString(err) + ").");

  _updateKernel = new cl::Kernel(_program, "updateParticles");
  _updateKernel->setArg(0, _gpu_buffers[0]);
  _updateKernel->setArg(1, _gpu_buffers[1]);
}

void CLContext::setParticles(size_t numParticles, char const *funcName) {
  cl::Kernel kernel(_program, funcName);

  kernel.setArg(0, _gpu_buffers[0]);
  kernel.setArg(1, _gpu_buffers[1]);

  glFinish();
  _queue.enqueueAcquireGLObjects(&_gpu_buffers);
  _queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(numParticles),
                              cl::NDRange(_maxWorkGroupSize));
  _queue.finish();
  _queue.enqueueReleaseGLObjects(&_gpu_buffers);
}

void CLContext::updateParticles(size_t numParticles,
                                glm::vec3 const gravityCenter,
                                cl_uchar const gravityEnabled) {
  // Needed to avoid CL_INVALID_ARG_SIZE error on some system
  cl_float3 toCLFormat;
  toCLFormat.s[0] = gravityCenter.x;
  toCLFormat.s[1] = gravityCenter.y;
  toCLFormat.s[2] = gravityCenter.z;

  _updateKernel->setArg(2, toCLFormat);
  _updateKernel->setArg(3, static_cast<float>(_gl.deltaTime));
  _updateKernel->setArg(4, gravityEnabled);

  glFinish();
  _queue.enqueueAcquireGLObjects(&_gpu_buffers);
  _queue.enqueueNDRangeKernel(*_updateKernel, cl::NullRange,
                              cl::NDRange(numParticles),
                              cl::NDRange(_maxWorkGroupSize));
  _queue.finish();
  _queue.enqueueReleaseGLObjects(&_gpu_buffers);
}

bool CLContext::_isExtensionSupported(std::string const &reqExt,
                                      std::string const &allExt) const {
  if (allExt.find(reqExt) != std::string::npos) return true;
  return false;
}

std::string const CLContext::getErrorString(cl_int const &error) {
  switch (error) {
    // Run-time and JIT compiler errors
    case 0:
      return "CL_SUCCESS";
    case -1:
      return "CL_DEVICE_NOT_FOUND";
    case -2:
      return "CL_DEVICE_NOT_AVAILABLE";
    case -3:
      return "CL_COMPILER_NOT_AVAILABLE";
    case -4:
      return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5:
      return "CL_OUT_OF_RESOURCES";
    case -6:
      return "CL_OUT_OF_HOST_MEMORY";
    case -7:
      return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8:
      return "CL_MEM_COPY_OVERLAP";
    case -9:
      return "CL_IMAGE_FORMAT_MISMATCH";
    case -10:
      return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11:
      return "CL_BUILD_PROGRAM_FAILURE";
    case -12:
      return "CL_MAP_FAILURE";
    case -13:
      return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14:
      return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15:
      return "CL_COMPILE_PROGRAM_FAILURE";
    case -16:
      return "CL_LINKER_NOT_AVAILABLE";
    case -17:
      return "CL_LINK_PROGRAM_FAILURE";
    case -18:
      return "CL_DEVICE_PARTITION_FAILED";
    case -19:
      return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // Compile-time errors
    case -30:
      return "CL_INVALID_VALUE";
    case -31:
      return "CL_INVALID_DEVICE_TYPE";
    case -32:
      return "CL_INVALID_PLATFORM";
    case -33:
      return "CL_INVALID_DEVICE";
    case -34:
      return "CL_INVALID_CONTEXT";
    case -35:
      return "CL_INVALID_QUEUE_PROPERTIES";
    case -36:
      return "CL_INVALID_COMMAND_QUEUE";
    case -37:
      return "CL_INVALID_HOST_PTR";
    case -38:
      return "CL_INVALID_MEM_OBJECT";
    case -39:
      return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40:
      return "CL_INVALID_IMAGE_SIZE";
    case -41:
      return "CL_INVALID_SAMPLER";
    case -42:
      return "CL_INVALID_BINARY";
    case -43:
      return "CL_INVALID_BUILD_OPTIONS";
    case -44:
      return "CL_INVALID_PROGRAM";
    case -45:
      return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46:
      return "CL_INVALID_KERNEL_NAME";
    case -47:
      return "CL_INVALID_KERNEL_DEFINITION";
    case -48:
      return "CL_INVALID_KERNEL";
    case -49:
      return "CL_INVALID_ARG_INDEX";
    case -50:
      return "CL_INVALID_ARG_VALUE";
    case -51:
      return "CL_INVALID_ARG_SIZE";
    case -52:
      return "CL_INVALID_KERNEL_ARGS";
    case -53:
      return "CL_INVALID_WORK_DIMENSION";
    case -54:
      return "CL_INVALID_WORK_GROUP_SIZE";
    case -55:
      return "CL_INVALID_WORK_ITEM_SIZE";
    case -56:
      return "CL_INVALID_GLOBAL_OFFSET";
    case -57:
      return "CL_INVALID_EVENT_WAIT_LIST";
    case -58:
      return "CL_INVALID_EVENT";
    case -59:
      return "CL_INVALID_OPERATION";
    case -60:
      return "CL_INVALID_GL_OBJECT";
    case -61:
      return "CL_INVALID_BUFFER_SIZE";
    case -62:
      return "CL_INVALID_MIP_LEVEL";
    case -63:
      return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64:
      return "CL_INVALID_PROPERTY";
    case -65:
      return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66:
      return "CL_INVALID_COMPILER_OPTIONS";
    case -67:
      return "CL_INVALID_LINKER_OPTIONS";
    case -68:
      return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // Extension errors
    case -1000:
      return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001:
      return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002:
      return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003:
      return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004:
      return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005:
      return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default:
      return "Unknown OpenCL error";
  }
}
