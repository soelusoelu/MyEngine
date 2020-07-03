#include "AssetsManager.h"
#include "../DebugLayer/Debug.h"
#include "../Mesh/FBX.h"
#include "../Mesh/OBJ.h"
#include "../Sound/SoundBase.h"
#include "../Sound/SourceVoice.h"
#include "../Sound/WAV.h"
#include "../System/Shader.h"
#include "../System/Texture.h"
#include "../System/World.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include <cassert>

AssetsManager::AssetsManager() :
    mSoundBase(std::make_unique<SoundBase>()) {
    assert(!mInstantiated);
    mInstantiated = true;
}

AssetsManager::~AssetsManager() {
    mInstantiated = false;
}

std::shared_ptr<Shader> AssetsManager::createShader(const std::string & fileName) {
    std::shared_ptr<Shader> shader = nullptr;
    auto itr = mShaders.find(fileName);
    if (itr != mShaders.end()) { //既に読み込まれている
        shader = itr->second;
    } else { //初読み込み
        shader = std::make_shared<Shader>(fileName);
        mShaders.emplace(fileName, shader);
    }
    return shader;
}

std::shared_ptr<Texture> AssetsManager::createTexture(const std::string & filePath, bool isSprite) {
    std::shared_ptr<Texture> texture = nullptr;
    auto itr = mTextures.find(filePath);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        if (isSprite) {
            World::instance().directory().setTextureDirectory(filePath);
        } else {
            //World::instance().directory().setModelDirectory(filePath);
        }
        auto fileName = FileUtil::getFileNameFromDirectry(filePath);
        texture = std::make_shared<Texture>(fileName);
        mTextures.emplace(filePath, texture);
    }
    return texture;
}

std::shared_ptr<SourceVoice> AssetsManager::createSound(const std::string& filePath, const SourceVoiceInitParam& param) {
    std::shared_ptr<SoundLoader> data = nullptr;
    auto itr = mSounds.find(filePath);
    if (itr != mSounds.end()) { //既に読み込まれている
        data = itr->second;
    } else { //初読み込み
        auto ext = FileUtil::getFileExtension(filePath);
        if (ext == ".wav") {
            data = std::make_shared<WAV>();
        } else {
            Debug::windowMessage(filePath + ": 対応していない拡張子です");
        }
        World::instance().directory().setSoundDirectory(filePath);
        auto fileName = FileUtil::getFileNameFromDirectry(filePath);
        data->loadFromFile(fileName);
        mSounds.emplace(filePath, data);
    }

    return mSoundBase->createSourceVoice(*data, param.flags, param.maxFrequencyRatio, param.callback, param.sendList, param.effectChain);
}

std::shared_ptr<IMeshLoader> AssetsManager::createMesh(const std::string & filePath) {
    std::shared_ptr<IMeshLoader> mesh = nullptr;
    auto itr = mMeshLoaders.find(filePath);
    if (itr != mMeshLoaders.end()) { //既に読み込まれている
        mesh = itr->second;
    } else { //初読み込み
        auto ext = FileUtil::getFileExtension(filePath);
        if (ext == ".obj") {
            mesh = std::make_shared<OBJ>();
        } else if (ext == ".fbx") {
            mesh = std::make_shared<FBX>();
        } else {
            Debug::windowMessage(filePath + ": 対応していない拡張子です");
        }
        World::instance().directory().setModelDirectory(filePath);
        auto fileName = FileUtil::getFileNameFromDirectry(filePath);
        mesh->perse(fileName);
        mMeshLoaders.emplace(filePath, mesh);
    }
    return mesh;
}
