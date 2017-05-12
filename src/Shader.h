#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <exception>
#include <string>
#include <tuple>

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
  void use();
  GLuint program() const noexcept { return this->program_; }

 private:
  std::tuple<std::string, std::string> getShaderSrcs(std::string vertexPath,
                                                     std::string fragmentPath);

  GLuint program_;
};

#endif
