#include "CLProgram.hpp"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "Tools.hpp"

int main() {
  try {
    Renderer renderer;

    ShaderProgram shaderProgram("./srcs/shaders/default.vs",
                                "./srcs/shaders/default.fs");

    CLProgram clProgram;

    std::cout << Tools::readFile("init.cl") << std::endl;

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                        0.0f,  0.0f,  0.5f, 0.0f};
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10.0f);

    double lastTime = 0.0;
    int nbFrames = 0;
    while (!glfwWindowShouldClose(renderer.getWindow())) {
      double currentTime = glfwGetTime();
      nbFrames++;
      if (currentTime - lastTime >= 0.25) {
        glfwSetWindowTitle(renderer.getWindow(),
                           std::to_string(nbFrames * 4).c_str());
        nbFrames = 0;
        lastTime += 0.25;
      }

      glfwPollEvents();
      if (renderer.inputs[GLFW_KEY_ESCAPE] == true)
        glfwSetWindowShouldClose(renderer.getWindow(), GL_TRUE);
      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      // vertices[3] += 0.0002f;
      // glBindBuffer(GL_ARRAY_BUFFER, VBO);
      // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
      // glBindBuffer(GL_ARRAY_BUFFER, 0);

      glUseProgram(shaderProgram.getID());
      glBindVertexArray(VAO);
      glDrawArrays(GL_POINTS, 0, 3);

      glfwSwapBuffers(renderer.getWindow());
    }
    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }
}
