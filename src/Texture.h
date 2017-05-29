#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

enum class DimensionsType : bool {
  Image = true,
  Custom = false,
};

struct Dimensions {
  Dimensions() : type(DimensionsType::Image), width(0), height(0) {}
  Dimensions(int width, int height)
      : type(DimensionsType::Custom), width(width), height(height) {}
  DimensionsType type;
  int width;
  int height;
};

struct TextureOptions {
  GLint wrapS{GL_REPEAT};
  GLint wrapT{GL_REPEAT};
  GLint internalFormat{GL_RGB};
  GLint imageFormat{GL_RGB};
  GLint filterMin{GL_LINEAR};
  GLint filterMag{GL_LINEAR};
};

class Texture {
 public:
  Texture(std::string path, const Dimensions &dimensions,
          const TextureOptions &options);
  Texture(unsigned char *data, const Dimensions &dimensions,
          const TextureOptions &options);
  ~Texture() { this->release(); }

  Texture() = delete;
  Texture(const Texture &) = delete;
  Texture &operator=(const Texture &) = delete;

  Texture(Texture &&other);
  Texture &operator=(Texture &&other);

  void bind() { glBindTexture(GL_TEXTURE_2D, texture_); }
  GLuint id() const noexcept { return texture_; }

 private:
  void release() {
    if (valid_) {
      glDeleteTextures(1, &texture_);
    }
  }
  GLuint texture_;
  bool valid_{true};
};

#endif
