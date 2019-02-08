#include "CLContext.hpp"
#include "GLRenderer.hpp"
#include "ShaderProgram.hpp"
#include "Tools.hpp"

int main() {
  try {
    GLRenderer gl;
    ShaderProgram shaderProgram("./srcs/shaders/default.vs",
                                "./srcs/shaders/default.fs");
    CLContext cl;
    cl.addSource(Tools::readFile("./srcs/kernels/init.cl"));
    cl.buildProgram();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, nullptr,
                 GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glBindVertexArray(0);

    cl_int err = 0;
    // Weird unexpected pattern in the display, check that out!
    // cl.initMemory(VBO);
    cl._gl_buffers.push_back(
        cl::BufferGL(cl._context, CL_MEM_READ_WRITE, VBO, &err));
    std::cout << CLContext::getErrorString(err) << std::endl;

    cl::Kernel kernel(cl._program, "initSphere", &err);
    std::cout << CLContext::getErrorString(err) << std::endl;

    err = kernel.setArg(0, cl._gl_buffers[0]);
    std::cout << "SetArg: " << CLContext::getErrorString(err) << std::endl;

    err = cl._queue.enqueueAcquireGLObjects(&cl._gl_buffers);
    std::cout << "Acquire GL Obj: " << CLContext::getErrorString(err)
              << std::endl;
    err = cl._queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(4),
                                         cl::NullRange);
    std::cout << CLContext::getErrorString(err) << std::endl;
    err = cl._queue.finish();
    std::cout << "Finish: " << CLContext::getErrorString(err) << std::endl;
    err = cl._queue.enqueueReleaseGLObjects(&cl._gl_buffers);
    std::cout << "Release GL Obj: " << CLContext::getErrorString(err)
              << std::endl;

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(4.0f);

    double lastTime = 0.0;
    int nbFrames = 0;
    while (!glfwWindowShouldClose(gl.getWindow())) {
      double currentTime = glfwGetTime();
      nbFrames++;
      if (currentTime - lastTime >= 0.25) {
        glfwSetWindowTitle(gl.getWindow(),
                           std::to_string(nbFrames * 4).c_str());
        nbFrames = 0;
        lastTime += 0.25;
      }

      glfwPollEvents();
      if (gl.inputs[GLFW_KEY_ESCAPE] == true)
        glfwSetWindowShouldClose(gl.getWindow(), GL_TRUE);
      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glBindVertexArray(VAO);
      glUseProgram(shaderProgram.getID());
      glDrawArrays(GL_POINTS, 0, 4);
      glBindVertexArray(0);
      glfwSwapBuffers(gl.getWindow());
    }
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
