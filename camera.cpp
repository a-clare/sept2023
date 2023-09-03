#include "camera.h"

Camera::Camera() {
  position_ = glm::vec3(0.0f, 0.0f, 0.0f);
  front_    = glm::vec3(0.0f, 0.0f, -1.0f);
  up_       = glm::vec3(0.0f, 1.0f, 0.0f);
  world_up_ = up_;
  zoom_     = 45.0f;
}

void Camera::Update() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front.y = sin(glm::radians(pitch_));
  front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  front_  = glm::normalize(front);
  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  right_ = glm::normalize(glm::cross(front_, world_up_));
  up_    = glm::normalize(glm::cross(right_, front_));
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::UpdateZoom(float yoffset) {
  zoom_ -= yoffset;
  if (zoom_ < 1.0f) {
    zoom_ = 1.0f;
  }
  if (zoom_ > 89.0f) {
    zoom_ = 89.0f;
  }
}