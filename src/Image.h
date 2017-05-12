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
    this->data_ = stbi_load(path.c_str(), &this->width_, &this->height_,
                            &this->nrComponents_, 0);
  }
  ~Image() { stbi_image_free(this->data_); }
  unsigned char *get() const noexcept { return this->data_; }
  int width() const noexcept { return this->width_; }
  int height() const noexcept { return this->height_; }
  int nrComponents() const noexcept { return this->nrComponents_; }

 private:
  unsigned char *data_;
  int width_;
  int height_;
  int nrComponents_;
};

#endif
