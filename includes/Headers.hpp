#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// OpenCL
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

// OpenGL
#include <glad/glad.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/OpenGL.h>
#elif _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif __linux__
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// GLM (math library)
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define EPSILON 0.001f

typedef struct {
  cl_uchar gravityEnabled = true;
  cl_float4 color;
} Settings;
