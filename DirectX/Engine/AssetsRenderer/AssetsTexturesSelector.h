#pragma once

#include "IAssetsRenderTexturesGetter.h"

//アセットテクスチャ選択クラス
class AssetsTexturesSelector {
public:
    AssetsTexturesSelector();
    ~AssetsTexturesSelector();
    void initialize(const IAssetsRenderTexturesGetter* getter);
    //マウスでテクスチャを選択する
    bool selectTexture(AssetsRenderTexturePtr& out);

private:
    AssetsTexturesSelector(const AssetsTexturesSelector&) = delete;
    AssetsTexturesSelector& operator=(const AssetsTexturesSelector&) = delete;

private:
    const IAssetsRenderTexturesGetter* mTexturesGetter;
};
