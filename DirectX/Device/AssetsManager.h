#pragma once

#include "../Mesh/IMeshLoader.h"
#include <memory>
#include <string>
#include <unordered_map>

class Shader;
class Sound;
class SoundBase;
class SoundLoader;
class Texture;

class AssetsManager {
public:
    AssetsManager();
    ~AssetsManager();
    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<Texture> createTexture(const std::string& fileName, bool isSprite = true);
    void createSound(std::shared_ptr<Sound>* sound, const std::string& fileName);
    std::shared_ptr<IMeshLoader> createMesh(const std::string& fileName);

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

private:
    std::unique_ptr<SoundBase> mSoundBase;

    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> mTextures;
    std::unordered_map<std::string, std::shared_ptr<SoundLoader>> mSounds;
    std::unordered_map<std::string, std::shared_ptr<IMeshLoader>> mMeshLoaders;

    static inline bool mInstantiated = false;
};
