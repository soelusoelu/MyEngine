#include "AssetsRenderTextureList.h"
#include "../../System/Window.h"
#include "../../System/Texture/MeshRenderOnTexture.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/LevelLoader.h"
#include <algorithm>
#include <iterator>

AssetsRenderTextureList::AssetsRenderTextureList()
    : mViewProj(Matrix4::identity)
    , mTextureSize(0)
    , mColumnDisplayLimit(0)
    , mTextureDisplayInterval(0)
{
    mViewProj = Matrix4::createLookAt(Vector3(0.f, 5.f, -5.f), Vector3::zero, Vector3::up)
        * Matrix4::createPerspectiveFOV(1.f, 45.f, 0.1f, 1000.f);
}

AssetsRenderTextureList::~AssetsRenderTextureList() = default;

void AssetsRenderTextureList::add(const std::string& fileName, const std::string& directoryPath) {
    const auto& filePath = directoryPath + fileName;

    //パスが読み込み済みなら終了
    if (loadedFilePath(filePath)) {
        return;
    }

    //テクスチャを追加
    createTexture(fileName, directoryPath);
    //パスを追加
    mTexturesFilePath.emplace(filePath);
}

const MeshRenderOnTexturePtrList& AssetsRenderTextureList::getTextures() const {
    return mTextures;
}

void AssetsRenderTextureList::initialize() {
    //ファイルから読み込んだパスでテクスチャを作成する
    for (const auto& path : mTexturesFilePath) {
        createTexture(path);
    }
    //メッシュをテクスチャに描画する
    drawMeshOnTexture();
}

void AssetsRenderTextureList::loadProperties(const rapidjson::Value& inObj) {
    const auto& artlObj = inObj["assetsRenderTextureList"];
    if (artlObj.IsObject()) {
        JsonHelper::getInt(artlObj, "textureSize", &mTextureSize);
        JsonHelper::getInt(artlObj, "textureDisplayInterval", &mTextureDisplayInterval);
        mColumnDisplayLimit = Window::width() / (mTextureSize + mTextureDisplayInterval);
        JsonHelper::getStringArray(artlObj, "texturesFilePath", &mTexturesFilePath);
    }
}

void AssetsRenderTextureList::saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
    rapidjson::Value props(rapidjson::kObjectType);
    JsonHelper::setInt(alloc, &props, "textureSize", mTextureSize);
    JsonHelper::setInt(alloc, &props, "textureDisplayInterval", mTextureDisplayInterval);
    JsonHelper::setStringArray(alloc, &props, "texturesFilePath", mTexturesFilePath);

    inObj.AddMember("assetsRenderTextureList", props, alloc);
}

void AssetsRenderTextureList::update() {
    //描画していないメッシュがなければ終了
    if (mNonDrawTextures.empty()) {
        return;
    }

    //mNonDrawTexturesの要素をmTexturesに追加する
    std::copy(mNonDrawTextures.begin(), mNonDrawTextures.end(), std::back_inserter(mTextures));
    mNonDrawTextures.clear();
}

void AssetsRenderTextureList::drawMeshOnTexture() const {
    //まだ描画していないメッシュをテクスチャに描画する
    for (const auto& tex : mNonDrawTextures) {
        tex->drawMeshOnTexture(mViewProj);
    }
}

void AssetsRenderTextureList::drawTexture(const Matrix4& proj) const {
    for (const auto& tex : mTextures) {
        tex->draw(proj);
    }
}

void AssetsRenderTextureList::createTexture(const std::string& fileName, const std::string& directoryPath) {
    const auto& newTex = std::make_shared<MeshRenderOnTexture>(fileName, directoryPath, mTextureSize, mTextureSize);

    //テクスチャの位置調整
    const auto TEXTURE_COUNT = mTextures.size() + mNonDrawTextures.size();
    newTex->setPositionForTexture(Vector2(
        (mTextureSize + mTextureDisplayInterval) * (TEXTURE_COUNT % mColumnDisplayLimit) + mTextureDisplayInterval,
        (mTextureSize + mTextureDisplayInterval) * (TEXTURE_COUNT / mColumnDisplayLimit) + Window::height() + mTextureDisplayInterval
    ));

    //テクスチャを追加
    mNonDrawTextures.emplace_back(newTex);
}

void AssetsRenderTextureList::createTexture(const std::string& filePath) {
    const auto& fileName = FileUtil::getFileNameFromDirectry(filePath);
    const auto& directoryPath = FileUtil::getDirectryFromFilePath(filePath);
    createTexture(fileName, directoryPath);
}

bool AssetsRenderTextureList::loadedFilePath(const std::string& filePath) const {
    return (mTexturesFilePath.find(filePath) != mTexturesFilePath.end());
}
