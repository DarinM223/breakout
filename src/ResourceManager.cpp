#include "ResourceManager.h"

void ResourceManager::loadShader(std::string vertexPath,
                                 std::string fragmentPath,
                                 const std::string& name) {
  Shader shader{std::move(vertexPath), std::move(fragmentPath)};
  this->shaders_.emplace(name, std::move(shader));
}

void ResourceManager::loadTexture(std::string path,
                                  const Dimensions& dimensions, bool alpha,
                                  const std::string& name) {
  TextureOptions options{};
  if (alpha) {
    options.internalFormat = GL_RGBA;
    options.imageFormat = GL_RGBA;
  }
  Texture texture{std::move(path), dimensions, options};
  this->textures_.emplace(name, std::move(texture));
}

const Shader& ResourceManager::getShader(const std::string& name) const {
  return this->shaders_.at(name);
}

const Texture& ResourceManager::getTexture(const std::string& name) const {
  return this->textures_.at(name);
}
