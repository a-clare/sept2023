#include "robot.h"
#include <glm/gtc/matrix_transform.hpp>

/* TODO: Use an EBO here? */
/* This creates a cube of 1x1x1 and we will scale it accordingly from the robot parameters */
static float robot_default_vertices_[] = {
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};

void Robot::Init() {
  if (!shader_.LoadShaderFromFile("/Users/adamclare/projects/sept2023/shaders/simple_shader.vs",
                                  "/Users/adamclare/projects/sept2023/shaders/simple_shader.fs")) {
    return;
  }
  shader_.Use();

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(robot_default_vertices_), robot_default_vertices_, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);

  UpdateModelMatrix();
}

void Robot::UpdateModelMatrix() {
  // TODO: Assert here if length/width/height < 0

  model_ = glm::translate(glm::mat4(1.0f), position_);

  // Since the default vertices are for a 1x1x1 cube, the robot l/w/h parameters can be used as
  // the scaling values (which are the diagonals of the model matrix)
  model_ = glm::scale(model_, glm::vec3(width_, length_, height_));

  /* We want the robot to appear at a height (Z) of 0, so its on top of the ground plane. To do this
   * we need to shift the robot up 1/2 of the height. If we dont do this, half of the robot will be below
   * the ground, and the other half above.
   * This is because the default vertices, which are centered on 0, go from -0.5 to 0.5,
   * so by default half of the vertices are below 0 (all the -0.5) */
//  Mat4f_SetValue(&robot_model_, 3, 2, 0.5f * (float)robot_height_);

  shader_.Use();
  shader_.SetMat4("model", model_);
}

void Robot::Draw() const {
  shader_.Use();
  shader_.SetVec4("color", 1, 0, 0, 1);
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 12*3);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableVertexAttribArray(0);
}
