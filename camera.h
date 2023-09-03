#ifndef SEPT2023__CAMERA_H_
#define SEPT2023__CAMERA_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * Made the Camera object a struct to force all members to be public.
 * Avoiding getters/setters cause Im lazy.
 * However I stuck with my naming convention of private naming (trailing underscore)
 */
struct Camera {
  Camera();
  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;
  // We are going to force a 2D camera but not allowing the yaw and pitch to change, only zoom
  // in and out
  const float yaw_ = -90.0f;
  const float pitch_ = 0;

  // These are mouse settings
  const float speed_       = 2.5f;
  const float sensitivity_ = 0.1f;
  float zoom_;

  /**
   * Update the internal camera vectors after changing any of the camera position/angles
   */
  void Update();

  /**
   * \return the View matrix, calculated from the internal camera vectors
   */
  glm::mat4 GetViewMatrix() const;

  /**
   * Update the zoom angle of the camera based on some offset
   * \param yoffset how much the user scrolled the mouse in the y direction
   */
  void UpdateZoom(float yoffset);
};

#endif
