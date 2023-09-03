#ifndef SEPT2023__SHADER_H_
#define SEPT2023__SHADER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct Shader {
  uint32_t id_ = 0;

  /**
   * \param vertexFile /path/to/some/vertex/file.vs
   * \param fragmentFile /path/to/some/fragment/file.vs
   * \return true if successfully loaded+compiled shaders
   * \return false if unable to open file
   * \return false if failed to compile shaders
   */
  bool LoadShaderFromFile(const std::string &vertexFile,
                          const std::string &fragmentFile);

  /**
   * Set the shader to be the active open GL shader
   */
  void Use() const;


  void SetBool(const std::string &name,
                       bool value) const;

  // These are all "setters" for any shader uniforms
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;
  void SetVec2(const std::string &name, const glm::vec2 &value) const;
  void SetVec2(const std::string &name, float x, float y) const;
  void SetVec3(const std::string &name, const glm::vec3 &value) const;
  void SetVec3(const std::string &name, float x, float y, float z) const;
  void SetVec4(const std::string &name, const glm::vec4 &value) const;
  void SetVec4(const std::string &name, float x, float y, float z, float w) const;
  void SetMat2(const std::string &name, const glm::mat2 &mat) const;
  void SetMat3(const std::string &name, const glm::mat3 &mat) const;
  void SetMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif
