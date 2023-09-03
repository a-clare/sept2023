#ifndef SEPT2023__ROBOT_H_
#define SEPT2023__ROBOT_H_

#include "shader.h"

/**
 * Helper struct for managing the drawing of a robot (a rectangle)
 */
struct Robot {
  Shader shader_;
  uint32_t vao_;
  uint32_t vbo_;

  glm::vec3 position_ = glm::vec3(0, 0, 0);
  float length_;
  float width_;
  // For now we only need 2D, so force the height to be 0
  const float height_ = 0;

  glm::mat4 model_;
  void Init();

  void UpdateModelMatrix();

  void Draw() const;
};

#endif
