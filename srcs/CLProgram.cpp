#include "CLProgram.hpp"

CLProgram::CLProgram(void) {
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  _defaultPlatform = platforms.front();
  std::vector<cl::Device> devices;
  _defaultPlatform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
  _defaultDevice = devices.front();

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
      (cl_context_properties)wglGetCurrentContext(),
      CL_WGL_HDC_KHR,
      (cl_context_properties)wglGetCurrentDC(),
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)_defaultPlatform,
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
      (cl_context_properties)glXGetCurrentContext(),
      CL_GLX_DISPLAY_KHR,
      (cl_context_properties)glXGetCurrentDisplay(),
      CL_CONTEXT_PLATFORM,
      (cl_context_properties)_defaultPlatform,
      0};
#endif

  _context = cl::Context(_defaultDevice, properties);
  _queue = cl::CommandQueue(_context, _defaultDevice);
}

CLProgram::~CLProgram(void) {}

bool CLProgram::_isExtensionSupported(std::string const &reqExt,
                                      std::string const &allExt) const {
  if (allExt.find(reqExt) != std::string::npos) return true;
  return false;
}