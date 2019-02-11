#include "CLContext.hpp"
#include "GLRenderer.hpp"
#include "ShaderProgram.hpp"

int main() {
  size_t numParticles = 4;
  try {
    GLRenderer gl;
    ShaderProgram shaderProgram("./srcs/shaders/default.vs",
                                "./srcs/shaders/default.fs");
    CLContext cl(gl);
    cl.addSource("init.cl");
    cl.buildProgram();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * numParticles, nullptr,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glBindVertexArray(0);

    cl.initMemory(VBO, numParticles);

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
      glDrawArrays(GL_POINTS, 0, numParticles);
      glBindVertexArray(0);

      glfwSwapBuffers(gl.getWindow());
    }
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
