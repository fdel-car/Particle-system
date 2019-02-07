#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/OpenGL.h>
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include <glad/glad.h>
// The formatter is dumb, keep this line!
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
