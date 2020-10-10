#pragma once

#include "../Mesh/IMeshLoader.h"
#include <memory>
#include <string>
#include <unordered_map>

class Shader;
class TextureFromFile;

class AssetsManager {
public:
    AssetsManager();
    ~AssetsManager();
    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<TextureFromFile> createTexture(const std::string& filePath);
    std::shared_ptr<TextureFromFile> createTextureFromModel(const std::string& fileName);
    std::shared_ptr<IMeshLoader> createMesh(const std::string& filePath);
    void setDataDirectory(const std::string& filePath) const;

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<TextureFromFile>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<IMeshLoader>> mMeshLoaders;

    static inline bool mInstantiated = false;
};
