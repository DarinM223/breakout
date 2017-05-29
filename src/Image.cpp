#include "Image.h"
#include "stb_image.h"

Image::Image(std::string path) {
  data_ = stbi_load(path.c_str(), &width_, &height_, &nrComponents_, 0);
}

Image::~Image() { stbi_image_free(data_); }
