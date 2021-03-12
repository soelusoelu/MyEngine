﻿#pragma once

#include "../../Component.h"
#include "../../../Math/Math.h"
#include "../../../Mesh/IAnimation.h"
#include "../../../Mesh/Motion.h"
#include <functional>
#include <string>
#include <vector>

class MeshShader;
class Subject;

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
    //このフレームでのボーン姿勢計算後に呼ばれるコールバック
    void callbackComputeCurrentBones(const std::function<void()>& callback);

private:
    SkinMeshComponent(const SkinMeshComponent&) = delete;
    SkinMeshComponent& operator=(const SkinMeshComponent&) = delete;

private:
    //アニメーションインターフェース
    IAnimation* mAnimation;
    //メッシュ用シェーダーコンポーネント
    std::shared_ptr<MeshShader> mMeshShader;
    //現在のボーン姿勢
    std::vector<Matrix4> mCurrentBones;
    //ボーン姿勢計算後に呼ばれるコールバック
    std::unique_ptr<Subject> mCallbackComputeCurrentBones;
    //現在のモーション番号
    int mCurrentMotionNo;
    //現在のモーションフレーム
    int mCurrentFrame;
    //Tポーズか
    bool mIsTPose;
};
