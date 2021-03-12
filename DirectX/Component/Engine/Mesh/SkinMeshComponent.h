﻿#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IAnimation.h"
#include "../../../Mesh/Motion.h"
#include <string>
#include <vector>

class MeshShader;

class SkinMeshComponent : public Component {
public:
    SkinMeshComponent(GameObject& gameObject);
    ~SkinMeshComponent();
    virtual void update() override;

    //モーション名を設定する
    void setMotionName(const std::string& name, unsigned motionNo);
    //モーション番号からモーションを変更する
    void changeMotion(unsigned motionNo);
    //モーション名からモーションを変更する
    void changeMotion(const std::string& motionName);
    //姿勢をTポーズにする
    void tPose();
    //モーション数を取得する
    int getMotionCount() const;
    //現在のモーションを取得する
    const Motion& getCurrentMotion() const;
    //現在のモーション番号を取得する
    int getCurrentMotionNumber() const;
    //現在のモーションのフレーム数を取得する
    int getCurrentMotionFrame() const;
    //現在のボーン姿勢行列を取得する
    const std::vector<Matrix4>& getBoneCurrentFrameMatrix() const;
    //各種インターフェースを設定する
    void setValue(const std::shared_ptr<MeshShader>& meshShader, IAnimation* anim);

private:
    SkinMeshComponent(const SkinMeshComponent&) = delete;
    SkinMeshComponent& operator=(const SkinMeshComponent&) = delete;

private:
    IAnimation* mAnimation;
    std::shared_ptr<MeshShader> mMeshShader;
    std::vector<Matrix4> mCurrentBones;
    int mCurrentMotionNo;
    int mCurrentFrame;
    bool mIsTPose;
};
