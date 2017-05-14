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
    this->program_ = other.program_;
    other.program_ = -1;
  }
  Shader &operator=(Shader &&other) {
    this->release();
    this->program_ = other.program_;
    other.program_ = -1;
    return *this;
  }

  void use() const noexcept { glUseProgram(this->program_); }
  GLuint program() const noexcept { return this->program_; }
  void setMatrix4(const char *name, const glm::mat4 &matrix) const;
  void setInteger(const char *name, GLint value) const;
  void setVector3(const char *name, const glm::vec3 &value) const;

 private:
  void release() { glDeleteProgram(this->program_); }
  std::tuple<std::string, std::string> getShaderSrcs(std::string vertexPath,
                                                     std::string fragmentPath);

  GLuint program_;
};

#endif
