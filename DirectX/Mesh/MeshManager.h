#pragma once

#include "IMeshAdder.h"
#include "IMeshesGetter.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>

class MeshRenderer;
class ShadowMap;

class MeshManager : public IMeshesGetter, public IMeshAdder {
public:
    MeshManager(bool forGame = true);
    ~MeshManager();
    virtual const MeshPtrList& getMeshes() const override;
    virtual void add(const MeshPtr& mesh, bool handleShadow) override;

    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj);
    void initialize();
    void update();
    void draw(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;
    void clear();
    void erase(const MeshPtr& mesh);

private:
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;

    //不要なメッシュを削除する
    void remove();
    //描画するか
    bool isDraw(const MeshRenderer& mesh) const;
    //メッシュの描画をする
    void drawMeshes(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;
    //テクスチャに影を描画する
    void drawShadow(
        const Matrix4& view,
        const Matrix4& projection,
        const Vector3& cameraPosition,
        const Vector3& dirLightDirection,
        const Vector3& dirLightColor
    ) const;

private:
    //影の影響を受けるメッシュリスト
    MeshPtrList mShadowMeshes;
    //影の影響を受けないメッシュリスト
    MeshPtrList mMeshes;
    //シャドーマップ
    std::unique_ptr<ShadowMap> mShadowMap;
    //ゲームシーン用のマネージャーか
    bool mForGame;
};
