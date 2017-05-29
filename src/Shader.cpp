#include "Shader.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

class ScopedShader {
 public:
  ScopedShader(GLuint shader) : shader_(shader) {}
  ~ScopedShader() { this->release(); }

  ScopedShader() = delete;
  ScopedShader(const ScopedShader&) = delete;
  ScopedShader& operator=(const ScopedShader&) = delete;

  ScopedShader(ScopedShader&& other) {
    shader_ = other.shader_;
    other.valid_ = false;
  }
  ScopedShader& operator=(ScopedShader&& other) {
    this->release();
    shader_ = other.shader_;
    other.valid_ = false;
    return *this;
  }

  GLuint get() const noexcept { return shader_; }

 private:
  void release() {
    if (valid_) {
      glDeleteShader(shader_);
    }
  }

  GLuint shader_;
  bool valid_{true};
};

ShaderLoadException::ShaderLoadException(std::string vertexPath,
                                         std::string fragmentPath) {
  what_ = "Could not load vertex shader file: " + std::move(vertexPath) +
          " and fragment shader file: " + std::move(fragmentPath);
}

ShaderCompileException::ShaderCompileException(Type type, std::string log) {
  std::string shaderTypeStr;
  switch (type) {
    case ShaderCompileException::Type::Vertex:
      shaderTypeStr = "vertex";
      break;
    case ShaderCompileException::Type::Fragment:
      shaderTypeStr = "fragment";
      break;
  }

  what_ = "Could not compile " + shaderTypeStr + " shader: " + std::move(log);
}

ProgramLinkException::ProgramLinkException(std::string log) {
  what_ = "Could not link program: " + std::move(log);
}

std::tuple<std::string, std::string> Shader::getShaderSrcs(
    std::string vertexPath, std::string fragmentPath) {
  std::ifstream vertexFile, fragmentFile;
  vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    vertexFile.close();
    fragmentFile.close();
    return std::make_tuple(vertexStream.str(), fragmentStream.str());
  } catch (std::ifstream::failure e) {
    throw ShaderLoadException{vertexPath, fragmentPath};
  }
}

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
  std::string vertexSrc, fragmentSrc;
  std::tie(vertexSrc, fragmentSrc) =
      this->getShaderSrcs(std::move(vertexPath), std::move(fragmentPath));
  const GLchar* vertexShaderSrc = vertexSrc.c_str();
  const GLchar* fragmentShaderSrc = fragmentSrc.c_str();

  GLint success;
  GLchar infoLog[512];

  ScopedShader vertexShader{glCreateShader(GL_VERTEX_SHADER)};
  glShaderSource(vertexShader.get(), 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader.get());
  glGetShaderiv(vertexShader.get(), GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader.get(), 512, NULL, infoLog);
    std::string log{infoLog};
    std::cout << "Vertex Log: " << log << "\n";
    throw ShaderCompileException{ShaderCompileException::Type::Vertex,
                                 std::move(log)};
  }

  ScopedShader fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
  glShaderSource(fragmentShader.get(), 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader.get());
  glGetShaderiv(fragmentShader.get(), GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader.get(), 512, NULL, infoLog);
    std::string log{infoLog};
    std::cout << "Fragment Log: " << log << "\n";
    throw ShaderCompileException{ShaderCompileException::Type::Fragment,
                                 std::move(log)};
  }

  program_ = glCreateProgram();
  glAttachShader(program_, vertexShader.get());
  glAttachShader(program_, fragmentShader.get());
  glLinkProgram(program_);
  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_, 512, NULL, infoLog);
    std::string log{infoLog};
    throw ProgramLinkException{std::move(log)};
  }
}

Shader::Shader(Shader&& other) {
  program_ = other.program_;
  other.valid_ = false;
}

Shader& Shader::operator=(Shader&& other) {
  this->release();
  program_ = other.program_;
  other.valid_ = false;
  return *this;
}

void Shader::setMatrix4(const char* name, const glm::mat4& matrix) {
  auto location = glGetUniformLocation(program_, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setInteger(const char* name, GLint value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1i(location, value);
}

void Shader::setFloat(const char* name, GLfloat value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform1f(location, value);
}

void Shader::setVector2(const char* name, const glm::vec2& value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform2f(location, value.x, value.y);
}

void Shader::setVector3(const char* name, const glm::vec3& value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform3f(location, value.x, value.y, value.z);
}

void Shader::setVector4(const char* name, const glm::vec4& value) {
  auto location = glGetUniformLocation(program_, name);
  glUniform4f(location, value.x, value.y, value.z, value.w);
}
