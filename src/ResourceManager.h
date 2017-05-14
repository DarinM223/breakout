#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include "Shader.h"
#include "Texture.h"

class ResourceManager {
 public:
  ResourceManager(){};

  void loadShader(std::string vertexPath, std::string fragmentPath,
                  const std::string& name);
  void loadTexture(std::string path, const Dimensions& dimensions, bool alpha,
                   const std::string& name);

  const Shader& getShader(const std::string& name) const;
  const Texture& getTexture(const std::string& name) const;

 private:
  std::unordered_map<std::string, Shader> shaders_;
  std::unordered_map<std::string, Texture> textures_;
};

#endif
