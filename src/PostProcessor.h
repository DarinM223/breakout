#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <GL/glew.h>
#include <exception>
#include "Shader.h"
#include "Texture.h"

struct MsfboInitException : public std::exception {
  const char* what() const noexcept override {
    return "Failed to initialize MSFBO";
  }
};

struct FboInitException : public std::exception {
  const char* what() const noexcept override {
    return "Failed to initialize FBO";
  }
};

class PostProcessor;

class RenderManager {
 public:
  RenderManager(PostProcessor& processor);  // Begin render.
  ~RenderManager();                         // End render.

 private:
  PostProcessor& processor_;
};

class PostProcessor {
 public:
  PostProcessor(Shader& shader, int width, int height);
  ~PostProcessor() { this->release(); }

  PostProcessor() = delete;
  PostProcessor(const PostProcessor&) = delete;
  PostProcessor& operator=(const PostProcessor&) = delete;

  PostProcessor(PostProcessor&& other);
  PostProcessor& operator=(PostProcessor&& other);

  RenderManager prepareRender() { return RenderManager{*this}; }
  void render(GLfloat time);

  GLboolean confuse{GL_FALSE};
  GLboolean chaos{GL_FALSE};
  GLboolean shake{GL_FALSE};

 private:
  friend class RenderManager;

  void release() {
    if (valid_) {
      glDeleteFramebuffers(1, &msfbo_);
      glDeleteFramebuffers(1, &fbo_);
      glDeleteRenderbuffers(1, &rbo_);
      glDeleteVertexArrays(1, &vao_);
    }
  }

  int width_;
  int height_;

  Shader& shader_;
  Texture texture_;
  GLuint msfbo_;
  GLuint fbo_;
  GLuint rbo_;
  GLuint vao_;
  bool valid_{true};
};

#endif
