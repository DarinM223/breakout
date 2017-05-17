#ifndef IMAGE_H
#define IMAGE_H

#include <exception>
#include <string>
#include "stb_image.h"

class ImageLoadException : public std::exception {
 public:
  const char *what() const noexcept override { return "Could not load image"; }
};

class Image {
 public:
  Image(std::string path) {
    data_ = stbi_load(path.c_str(), &width_, &height_, &nrComponents_, 0);
  }
  ~Image() { stbi_image_free(data_); }
  unsigned char *get() const noexcept { return data_; }
  int width() const noexcept { return width_; }
  int height() const noexcept { return height_; }
  int nrComponents() const noexcept { return nrComponents_; }

 private:
  unsigned char *data_;
  int width_;
  int height_;
  int nrComponents_;
};

#endif
