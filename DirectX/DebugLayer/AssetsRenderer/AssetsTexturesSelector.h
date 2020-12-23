#pragma once

#include "IAssetsRenderTexturesGetter.h"

//アセットテクスチャ選択クラス
class AssetsTexturesSelector {
public:
    AssetsTexturesSelector();
    ~AssetsTexturesSelector();
    void initialize(const IAssetsRenderTexturesGetter* getter);
    //マウスで選択したテクスチャのファイルパスを返す
    bool selectTexture(MeshRenderOnTexturePtr& out);

private:
    AssetsTexturesSelector(const AssetsTexturesSelector&) = delete;
    AssetsTexturesSelector& operator=(const AssetsTexturesSelector&) = delete;

private:
    const IAssetsRenderTexturesGetter* mTexturesGetter;
};
