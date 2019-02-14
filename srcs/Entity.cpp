#include "Entity.hpp"

Entity::Entity(glm::vec3 position, glm::vec3 eulerAngles)
    : _position(position), _eulerAngles(eulerAngles) {
  _updateTransformMatrices();
  _updateModelMatrix();
}

Entity::~Entity(void) {}

void Entity::update(void) {}

glm::vec3 const &Entity::getPosition(void) const { return _position; }

glm::mat4 const &Entity::getModelMatrix(void) const { return _modelMatrix; }

glm::vec3 const &Entity::getEulerAngles() const {
  return _eulerAngles;  // Could be false since rotate(...) does not update
                        // them.
}

void Entity::translate(glm::vec3 translation) {
  _translationMatrix = glm::translate(_translationMatrix, translation);
  _updateModelMatrix();
  _position.x = _modelMatrix[3][0];
  _position.y = _modelMatrix[3][1];
  _position.z = _modelMatrix[3][2];
}

void Entity::scale(glm::vec3 scale) {
  _scaleMatrix = glm::scale(_scaleMatrix, scale);
  _updateModelMatrix();
}

// Avoid to use that since the euler angles are not updated.
void Entity::rotate(float angle, glm::vec3 axis) {
  _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(angle), axis);
  // TODO: Update euler angles correctly
  _updateModelMatrix();
}

void Entity::rotateX(float angle) {
  _rotationMatrix = glm::rotate(_rotationMatrix, glm::radians(angle),
                                glm::vec3(1.0, 0.0, 0.0));
  _eulerAngles.x = fmod(_eulerAngles.x + angle, 360.0f);
  _updateModelMatrix();
}

void Entity::rotateY(float angle) {
  glm::mat4 tmp = glm::rotate(glm::mat4(1.0f), glm::radians(angle),
                              glm::vec3(0.0, 1.0, 0.0));
  _rotationMatrix = tmp * _rotationMatrix;
  _eulerAngles.y = fmod(_eulerAngles.y + angle, 360.0f);
  _updateModelMatrix();
}

// Once more be careful, _eulerAngles could not be up to date.
void Entity::_updateTransformMatrices() {
  _translationMatrix[3][0] = _position.x;
  _translationMatrix[3][1] = _position.y;
  _translationMatrix[3][2] = _position.z;
  _rotationMatrix = glm::mat4(glm::quat(glm::radians(_eulerAngles)));
  // TODO: _scaleMatrix update
  _updateModelMatrix();
}

void Entity::_updateModelMatrix(void) {
  if (_localOrientation)
    _modelMatrix = _translationMatrix * _rotationMatrix * _scaleMatrix;
  else
    _modelMatrix = _rotationMatrix * _translationMatrix * _scaleMatrix;
}
