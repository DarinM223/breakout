#include "Texture.h"
#include "Image.h"

Texture::Texture(std::string path, std::optional<Dimensions> dimensions,
                 GLint wrapS, GLint wrapT, GLint internalFormat,
                 GLint imageFormat, GLint filterMin, GLint filterMag) {
  glGenTextures(1, &this->texture_);
  glBindTexture(GL_TEXTURE_2D, this->texture_);
  {
    Image image{std::move(path)};
    if (image.get() == nullptr) {
      throw ImageLoadException{};
    }

    int width, height;
    if (auto unwrappedDimensions = dimensions) {
      width = unwrappedDimensions->width;
      height = unwrappedDimensions->height;
    } else {
      width = image.width();
      height = image.height();
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
                 imageFormat, GL_UNSIGNED_BYTE, image.get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, this->texture_); }
