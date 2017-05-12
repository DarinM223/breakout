#include "Shader.h"
#include <fstream>
#include <memory>
#include <sstream>

class ScopedShader {
 public:
  ScopedShader(GLuint shader) : shader_(shader) {}
  ~ScopedShader() { glDeleteShader(this->shader_); }
  GLuint get() const noexcept { return this->shader_; }

 private:
  GLuint shader_;
};

const char* ShaderLoadException::what() const noexcept {
  std::ostringstream out;
  out << "Could not load vertex shader file: " << this->vertexPath_
      << " and fragment shader file: " << this->fragmentPath_;
  return out.str().c_str();
}

const char* ShaderCompileException::what() const noexcept {
  std::string shaderTypeStr;
  switch (this->type_) {
    case ShaderCompileException::Type::Vertex:
      shaderTypeStr = "vertex";
      break;
    case ShaderCompileException::Type::Fragment:
      shaderTypeStr = "fragment";
      break;
  }
  std::ostringstream out;
  out << "Could not compile " << shaderTypeStr << " shader: " << this->log_;
  return out.str().c_str();
}

const char* ProgramLinkException::what() const noexcept {
  return "Could not link program";
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
    throw ShaderCompileException{ShaderCompileException::Type::Fragment,
                                 std::move(log)};
  }

  this->program_ = glCreateProgram();
  glAttachShader(this->program_, vertexShader.get());
  glAttachShader(this->program_, fragmentShader.get());
  glLinkProgram(this->program_);
  glGetProgramiv(this->program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->program_, 512, NULL, infoLog);
    std::string log{infoLog};
    throw ProgramLinkException{std::move(log)};
  }
}

void Shader::use() { glUseProgram(this->program_); }
