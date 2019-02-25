#pragma once

#include "Entity.hpp"
#include "GLRenderer.hpp"

class Camera : public Entity {
 public:
  static glm::vec3 const initialPos;

  Camera(glm::vec3 const &pos, glm::vec3 const &eulerAngles,
         GLRenderer const &gl);
  virtual ~Camera(void);

  glm::mat4 const &getViewMatrix(void) const;
  glm::mat4 const &getProjectionMatrix(void) const;

  virtual void update(void);
  virtual void translate(glm::vec3 translation);
  virtual void rotateX(float angle);
  virtual void rotateY(float angle);

  bool isInFreeNavMode(void) const;
  void toggleFreeNavMode(void);

  void mouseCallback(GLFWwindow *window, double xpos, double ypos);

 private:
  bool _freeNavMode = false;
  float _speed;
  float _aspectRatio;
  glm::mat4 _view;
  glm::mat4 _projection;
  glm::vec2 _lastMousePos;
  glm::vec3 _front;
  glm::vec3 _right;
  glm::vec3 _up;
  GLRenderer const &_gl;

  Camera(void);
  Camera(Camera const &src);

  void _updateData(void);

  Camera &operator=(Camera const &rhs);
};
