#pragma once

#include "../Mesh/IMeshLoader.h"
#include "../Sound/SourceVoiceInitParam.h"
#include <memory>
#include <string>
#include <unordered_map>

class Shader;
class SoundBase;
class SoundLoader;
class SourceVoice;
class Texture;

class AssetsManager {
public:
    AssetsManager();
    ~AssetsManager();
    std::shared_ptr<Shader> createShader(const std::string& fileName);
    std::shared_ptr<Texture> createTexture(const std::string& filePath, bool isSprite = true);
    std::shared_ptr<SourceVoice> createSound(const std::string& filePath, const SourceVoiceInitParam& param);
    std::shared_ptr<IMeshLoader> createMesh(const std::string& filePath);

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
