#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <glad/glad.h>
// The formatter is dumb, keep this line!
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
