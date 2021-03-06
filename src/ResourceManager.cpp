#include "ResourceManager.h"

void ResourceManager::loadShader(std::string vertexPath,
                                 std::string fragmentPath,
                                 const std::string& name) {
  Shader shader{std::move(vertexPath), std::move(fragmentPath)};
  shaders_.emplace(name, std::move(shader));
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
  textures_.emplace(name, std::move(texture));
}

Shader& ResourceManager::getShader(const std::string& name) {
  return shaders_.find(name)->second;
}

Texture& ResourceManager::getTexture(const std::string& name) {
  return textures_.find(name)->second;
}
