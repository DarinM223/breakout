#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <optional>
#include <string>

struct Dimensions {
  int width;
  int height;
};

class Texture {
 public:
  Texture(std::string path, std::optional<Dimensions> dimensions = {},
          GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT,
          GLint internalFormat = GL_RGB, GLint imageFormat = GL_RGB,
          GLint filterMin = GL_NEAREST_MIPMAP_LINEAR,
          GLint filterMag = GL_NEAREST_MIPMAP_LINEAR);
  void bind();

 private:
  GLuint texture_;
};

#endif
