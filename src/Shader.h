#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <exception>
#include <string>
#include <tuple>
#include "glm/glm.hpp"

class ShaderLoadException : public std::exception {
 public:
  ShaderLoadException(std::string vertexPath, std::string fragmentPath)
      : vertexPath_(std::move(vertexPath)),
        fragmentPath_(std::move(fragmentPath)) {}
  const char *what() const noexcept override;

 private:
  std::string vertexPath_;
  std::string fragmentPath_;
};

class ShaderCompileException : public std::exception {
 public:
  enum class Type : bool { Vertex = false, Fragment = true };

  ShaderCompileException(Type type, std::string log)
      : type_(type), log_(std::move(log)) {}
  const char *what() const noexcept override;

 private:
  Type type_;
  std::string log_;
};

class ProgramLinkException : public std::exception {
 public:
  ProgramLinkException(std::string log) : log_(std::move(log)) {}
  const char *what() const noexcept override;

 private:
  std::string log_;
};

class Shader {
 public:
  Shader(std::string vertexPath, std::string fragmentPath);
  ~Shader() { this->release(); }

  Shader() = delete;
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  Shader(Shader &&other) {
    program_ = other.program_;
    other.valid_ = false;
  }
  Shader &operator=(Shader &&other) {
    this->release();
    program_ = other.program_;
    other.valid_ = false;
    return *this;
  }

  void use() noexcept { glUseProgram(program_); }
  GLuint program() const noexcept { return program_; }
  void setMatrix4(const char *name, const glm::mat4 &matrix);
  void setInteger(const char *name, GLint value);
  void setFloat(const char *name, GLfloat value);
  void setVector2(const char *name, const glm::vec2 &value);
  void setVector3(const char *name, const glm::vec3 &value);
  void setVector4(const char *name, const glm::vec4 &value);

 private:
  void release() {
    if (valid_) {
      glDeleteProgram(program_);
    }
  }

  std::tuple<std::string, std::string> getShaderSrcs(std::string vertexPath,
                                                     std::string fragmentPath);

  GLuint program_;
  bool valid_{true};
};

#endif
