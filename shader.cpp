#include "shader.h"

/**
 * This will print an error message if unable to open file
 * \param file /path/to/some/shader/file (vertex or fragment)
 * \param shaderCode this is a return variable, will hold the source code for the shader after reading the file
 * \return true if successful
 * \return false if unable to open file
 */
static bool LoadShaderFile(const std::string &file,
                           std::string &shaderCode) {

  std::ifstream shader_file(file, std::ios::in);
  if (!shader_file.is_open()) {
    std::cout << "ERROR (Shader): Could not open file: " << file << std::endl;
    return false;
  }
  // We are going to get the size of the file and allocate the shader source string,
  // then read all the contents into the allocated buffer.
  // Did this instead of doing the std:: stringstream approach which can be a bit slower for larger files
  shader_file.seekg(0, std::ios::end);
  size_t file_size = shader_file.tellg();
  shader_file.seekg(0);

  shaderCode.resize(file_size, ' ');
  shader_file.read(&shaderCode[0], file_size);
  return true;
}

bool Shader::LoadShaderFromFile(const std::string &vertexFile,
                                const std::string &fragmentFile) {
  // Put the shader program in an invalid state
  id_ = 0;

  std::string vertex_source, fragment_source;
  if (!LoadShaderFile(vertexFile, vertex_source)) {
    return false;
  }
  if (!LoadShaderFile(fragmentFile, fragment_source)) {
    return false;
  }

  std::int32_t shader_success_flag = 0;
  char shader_error_log[512];

  // OpenGL requires a pointer to a const char*, so we need to break
  // out the c_str() from the shader source string object, so we
  // can later than use this (shader_source) to pass the pointer to OpenGL
  const char *shader_source = vertex_source.c_str();

  // Try and create the vertex shader
  std::uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &shader_source, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_success_flag);
  if (!shader_success_flag) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, shader_error_log);
    std::cout << "ERROR (Shader): Failed compiling vertex shader " << shader_error_log << std::endl;
    return false;
  }

  shader_source = fragment_source.c_str();
  // Try and create the vertex shader
  std::uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &shader_source, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_success_flag);
  if (!shader_success_flag) {
    glGetShaderInfoLog(fragment_shader, 512, NULL, shader_error_log);
    std::cout << "ERROR (Shader): Failed compiling fragment shader " << shader_error_log << std::endl;
    return false;
  }

  id_ = glCreateProgram();
  glAttachShader(id_, vertex_shader);
  glAttachShader(id_, fragment_shader);
  glLinkProgram(id_);
  glGetProgramiv(id_, GL_LINK_STATUS, &shader_success_flag);
  if (!shader_success_flag) {
    glGetProgramInfoLog(id_, 512, NULL, shader_error_log);
    std::cout << "ERROR (Shader): Failed compiling shader progam " << shader_error_log << std::endl;
    return false;
  }
  // Once linked we can delete the vertex/fragment shaders
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return true;

}

void Shader::Use() const {
  glUseProgram(id_);
}

void Shader::SetBool(const std::string &name,
             bool value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name,
                    int value) const {
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name,
                      float value) const {
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec2(const std::string &name,
                     const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string &name,
                     float x,
                     float y) const {
  glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string &name,
                     const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string &name,
                     float x,
                     float y,
                     float z) const {
  glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string &name,
                     const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string &name,
                     float x,
                     float y,
                     float z,
                     float w) const {
  glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string &name,
                     const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name,
                     const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name,
                     const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}