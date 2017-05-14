#include "Texture.h"
#include "Image.h"

Texture::Texture(std::string path, const Dimensions& dimensions,
                 const TextureOptions& options) {
  glGenTextures(1, &this->texture_);
  glBindTexture(GL_TEXTURE_2D, this->texture_);
  {
    Image image{std::move(path)};
    if (image.get() == nullptr) {
      throw ImageLoadException{};
    }

    int width, height;
    if (dimensions.type == DimensionsType::Custom) {
      width = dimensions.width;
      height = dimensions.height;
    } else {
      width = image.width();
      width = image.height();
    }
    glTexImage2D(GL_TEXTURE_2D, 0, options.internalFormat, width, height, 0,
                 options.imageFormat, GL_UNSIGNED_BYTE, image.get());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, options.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, options.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, options.filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, options.filterMag);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}
