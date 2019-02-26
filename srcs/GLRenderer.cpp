#include "GLRenderer.hpp"

GLRenderer::GLRenderer(void) {
  if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
  // glfwWindowHint(GLFW_SAMPLES, 4);
  _window =
      glfwCreateWindow(WIDTH, HEIGHT, "Particle system", nullptr, nullptr);
  if (!_window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create windows GLFW");
  }

  // Discreetly center window
  glfwHideWindow(_window);
  _centerWindow(glfwGetPrimaryMonitor());
  glfwShowWindow(_window);

  glfwGetFramebufferSize(_window, &_width, &_height);
  glfwMakeContextCurrent(_window);
  // glfwSwapInterval(0);  // Remove 60 fps limit from glfw

  glfwSetKeyCallback(_window, _keyCallback);
  glfwSetCursorPosCallback(_window, _cursorCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to initialize GLAD");
  printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
  glViewport(0, 0, _width, _height);

  _initShader();
}

GLRenderer::~GLRenderer(void) {
  if (_shaderProgram) delete _shaderProgram;
  glfwTerminate();
}

// https://vallentin.io/2014/02/07/glfw-center-window
void GLRenderer::_centerWindow(GLFWmonitor *monitor) {
  if (!monitor) return;
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  if (!mode) return;
  int monitorX, monitorY;
  glfwGetMonitorPos(monitor, &monitorX, &monitorY);
  int windowWidth, windowHeight;
  glfwGetWindowSize(_window, &windowWidth, &windowHeight);
  glfwSetWindowPos(_window, monitorX + (mode->width - windowWidth) / 2,
                   monitorY + (mode->height - windowHeight) / 2);
}

void GLRenderer::_initShader(void) {
  _shaderProgram = new ShaderProgram("./srcs/shaders/default.vs",
                                     "./srcs/shaders/default.fs");
}

void GLRenderer::updateMousePos(void) const {
  double xPos, yPos;
  glfwGetCursorPos(_window, &xPos, &yPos);
  _mousePos = glm::vec2(xPos, yPos);
}

void GLRenderer::initMemory(size_t numParticles) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * numParticles, nullptr,
               GL_DYNAMIC_DRAW);

  // Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), nullptr);

  // Velocities
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle),
                        (void *)(offsetof(Particle, velocity)));

  glBindVertexArray(0);
}

GLFWwindow *GLRenderer::getWindow(void) const { return _window; }

int GLRenderer::getWidth(void) const { return _width; }

int GLRenderer::getHeight(void) const { return _height; }

glm::vec2 const GLRenderer::getMousePos(void) const { return _mousePos; }

bool GLRenderer::isKeyPressed(int keyID) const {
  auto result = _keyboardMap.find(keyID);
  return result != _keyboardMap.end() && result->second.currFrame;
}

bool GLRenderer::isKeyJustPressed(int keyID) const {
  auto result = _keyboardMap.find(keyID);
  return result != _keyboardMap.end() && result->second.currFrame &&
         !result->second.prevFrame;
}

void GLRenderer::displayParticles(size_t numParticles, glm::mat4 const &VP,
                                  glm::vec3 const &gravityCenter,
                                  glm::vec3 const &satColor) {
  // Compute deltaTime
  currentTime = glfwGetTime();
  deltaTime = currentTime - lastTime;
  lastTime = currentTime;

  // Update FPS counter
  _nbFrames++;
  if (currentTime - lastFPSUpdateTime >= 1.0) {
    // Modify the string to have a space every three digits for lisibility
    std::string str = std::to_string(numParticles);
    int idx = 0;
    for (auto it = str.end(); it != str.begin(); it--, idx++) {
      if (idx == 3) {
        str.insert(std::distance(str.begin(), it), 1, ' ');
        idx = 0;
      }
    }

    glfwSetWindowTitle(
        _window, (str + " : " + std::to_string(_nbFrames) + " FPS").c_str());
    _nbFrames = 0;
    lastFPSUpdateTime += 1.0;
  }

  // Update inputs
  for (auto &key : _keyboardMap) key.second.prevFrame = key.second.currFrame;

  glBindVertexArray(VAO);
  glUseProgram(_shaderProgram->getID());
  _shaderProgram->setMat4("VP", VP);
  _shaderProgram->setVec3("gravityCenter", gravityCenter);
  _shaderProgram->setVec3("satColor", satColor);
  glDrawArrays(GL_POINTS, 0, numParticles);
  glBindVertexArray(0);
}

void GLRenderer::switchCursorMode(bool freeNavMode) const {
  glfwSetInputMode(_window, GLFW_CURSOR,
                   freeNavMode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

glm::vec2 GLRenderer::_mousePos = glm::vec2(0.0f);

void GLRenderer::_cursorCallback(GLFWwindow *window, double xPos, double yPos) {
  _mousePos.x = xPos;
  _mousePos.y = yPos;
  (void)window;
}

std::map<int, KeyState> GLRenderer::_keyboardMap = std::map<int, KeyState>();

void GLRenderer::_keyStateUpdate(int keyID, bool isPressed) {
  if (_keyboardMap.find(keyID) == _keyboardMap.end()) {
    KeyState keyState;
    keyState.prevFrame = false;
    _keyboardMap[keyID] = keyState;
  }
  _keyboardMap[keyID].currFrame = isPressed;
}

void GLRenderer::_keyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_RELEASE) {
    _keyStateUpdate(key, action == GLFW_PRESS);
  }
  (void)scancode;
  (void)window;
  (void)mods;
}
