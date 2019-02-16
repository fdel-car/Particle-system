#include "Camera.hpp"

Camera::Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
               GLRenderer const &gl)
    : Entity(pos, eulerAngles), _gl(gl) {
  _speed = 2.0f;
  _updateData();
  _aspectRatio =
      static_cast<float>(_gl.getWidth()) / static_cast<float>(_gl.getHeight());
  _projection =
      glm::perspective(glm::radians(30.0f), _aspectRatio, 0.1f, 100.0f);
}

bool Camera::isInDebugMode(void) const { return _debugMode; }

Camera::~Camera(void) {}

glm::mat4 const &Camera::getViewMatrix(void) const { return _view; };

glm::mat4 const &Camera::getProjectionMatrix(void) const { return _projection; }

void Camera::update(void) {
  updateDebugMode();
  if (!_debugMode) return;
  float deltaTime = _gl.deltaTime;

  if (_gl.isKeyPressed(GLFW_KEY_W)) translate(_front * deltaTime * _speed);
  if (_gl.isKeyPressed(GLFW_KEY_D)) translate(_right * deltaTime * _speed);
  if (_gl.isKeyPressed(GLFW_KEY_S)) translate(-_front * deltaTime * _speed);
  if (_gl.isKeyPressed(GLFW_KEY_A)) translate(-_right * deltaTime * _speed);
  if (_gl.isKeyPressed(GLFW_KEY_SPACE)) translate(_up * deltaTime * _speed);
  if (_gl.isKeyPressed(GLFW_KEY_C)) translate(-_up * deltaTime * _speed);

  glm::vec2 const mousePos = _gl.getMousePos();
  float xOffset = mousePos.x - _lastMousePos.x;
  float yOffset =
      _lastMousePos.y -
      mousePos.y;  // Reversed since y-coordinates range from bottom to top
  _lastMousePos.x = mousePos.x;
  _lastMousePos.y = mousePos.y;
  float sensitivity = 0.05f;
  xOffset *= sensitivity;
  yOffset *= sensitivity;
  if (glm::epsilonNotEqual(0.0f, yOffset, EPSILON)) rotateX(yOffset);
  if (glm::epsilonNotEqual(0.0f, xOffset, EPSILON)) rotateY(-xOffset);
}

void Camera::updateDebugMode(void) {
  if (_gl.isKeyJustPressed(GLFW_KEY_TAB)) {
    _debugMode = !_debugMode;
    _gl.switchCursorMode(_debugMode);

    // Avoid camera jump on first frame
    _gl.updateMousePos();
    _lastMousePos.x = _gl.getMousePos().x;
    _lastMousePos.y = _gl.getMousePos().y;

    if (!_debugMode) {
      _position = glm::vec3(0.0, 0.0, 5.0);
      _eulerAngles = glm::vec3(0.0f);
      _updateTransformMatrices();
      _updateData();
    }
  }
}

void Camera::translate(glm::vec3 translation) {
  Entity::translate(translation);
  _view = glm::inverse(getModelMatrix());
}

void Camera::rotateX(float angle) {
  Entity::rotateX(angle);
  _updateData();
}

void Camera::rotateY(float angle) {
  Entity::rotateY(angle);
  _updateData();
}

void Camera::_updateData(void) {
  _right = glm::normalize(glm::vec3(
      getModelMatrix()[0][0], getModelMatrix()[0][1], getModelMatrix()[0][2]));
  _front = -glm::normalize(glm::vec3(
      getModelMatrix()[2][0], getModelMatrix()[2][1], getModelMatrix()[2][2]));
  _up = glm::normalize(glm::cross(_right, _front));
  _view = glm::inverse(getModelMatrix());
}
