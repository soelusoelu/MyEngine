﻿#include "AssetsManager.h"
#include "../DebugLayer/Debug.h"
#include "../Mesh/FBX.h"
#include "../Mesh/OBJ.h"
#include "../System/Shader.h"
#include "../System/Texture.h"
#include "../System/World.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include <cassert>

AssetsManager::AssetsManager() {
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
        World::instance().directory().setShaderDirectory();
        shader = std::make_shared<Shader>(fileName);
        mShaders.emplace(fileName, shader);
    }
    return shader;
}

std::shared_ptr<Texture> AssetsManager::createTexture(const std::string & filePath) {
    std::shared_ptr<Texture> texture = nullptr;
    auto itr = mTextures.find(filePath);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        World::instance().directory().setTextureDirectory(filePath);
        auto fileName = FileUtil::getFileNameFromDirectry(filePath);
        texture = std::make_shared<Texture>(fileName);
        mTextures.emplace(filePath, texture);
    }
    return texture;
}

std::shared_ptr<Texture> AssetsManager::createTextureFromModel(const std::string& fileName) {
    std::shared_ptr<Texture> texture = nullptr;
    auto itr = mTextures.find(fileName);
    if (itr != mTextures.end()) { //既に読み込まれている
        texture = itr->second;
    } else { //初読み込み
        //モデルからテクスチャを生成したい場合、事前にディレクトリを移動させている必要がある
        texture = std::make_shared<Texture>(fileName);
        mTextures.emplace(fileName, texture);
    }
    return texture;
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

void AssetsManager::setDataDirectory(const std::string& filePath) const {
    const auto& dir = World::instance().directory();
    dir.setDataDirectory(filePath);
}
