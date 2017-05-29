#ifndef IMAGE_H
#define IMAGE_H

#include <exception>
#include <string>

class ImageLoadException : public std::exception {
 public:
  const char *what() const noexcept override { return "Could not load image"; }
};

class Image {
 public:
  Image(std::string path);
  ~Image();
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
