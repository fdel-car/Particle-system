#pragma once

#include "Headers.hpp"

class Entity {
 public:
  Entity(glm::vec3 position, glm::vec3 eulerAngles);
  virtual ~Entity(void);

  glm::vec3 const &getPosition(void) const;
  glm::vec3 const &getEulerAngles(void) const;
  glm::mat4 const &getModelMatrix(void) const;

  virtual void update(void);

  void updateModel(void);
  void scale(glm::vec3 scale);
  void rotate(float angle, glm::vec3 axis);
  void rotateX(float angle);
  void rotateY(float angle);
  virtual void translate(glm::vec3 translation);

 private:
  glm::vec3 _position;
  glm::vec3 _eulerAngles;

  glm::mat4 _scaleMatrix = glm::mat4(1.0f);
  glm::mat4 _rotationMatrix = glm::mat4(1.0f);
  glm::mat4 _translationMatrix = glm::mat4(1.0f);
  glm::mat4 _modelMatrix = glm::mat4(1.0f);

  Entity(Entity const &src);
  Entity &operator=(Entity const &rhs);

  void _updateModelMatrix(void);

 protected:
  bool _localOrientation = true;

  Entity(void);
};
