#pragma once

#include "../../Math/Math.h"

class SimpleCamera {
public:
    SimpleCamera();
    ~SimpleCamera();

    //ビュー行列を設定する
    void setView(const Matrix4& view);
    //ビュー行列を取得する
    const Matrix4& getView() const;
    //射影行列を設定する
    void setProjection(const Matrix4& projection);
    //射影行列を取得する
    const Matrix4& getProjection() const;
    // View * Projection
    Matrix4 getViewProjection() const;
    //位置を設定する
    void setPosition(const Vector3& pos);
    //位置を取得する
    Vector3 getPosition() const;
    //視点を設定する
    void lookAt(const Vector3& position);
    //視点を取得する
    const Vector3& getLookAt() const;
    //視野角を設定する
    void setFov(float fov);
    //視野角を取得する
    float getFov() const;
    //ニアクリップを設定する
    void setNearClip(float nearClip);
    //ニアクリップを取得する
    float getNearClip() const;
    //ファークリップを設定する
    void setFarClip(float farClip);
    //ファークリップを取得する
    float getFarClip() const;

private:
    SimpleCamera(const SimpleCamera&) = delete;
    SimpleCamera& operator=(const SimpleCamera&) = delete;

    //ビュー行列を計算する
    void calcView();
    //射影行列を計算する
    void calcProjection();

private:
    Matrix4 mView;
    Matrix4 mProjection;
    Vector3 mPosition;
    Vector3 mLookAt;
    Vector3 mUp;
    float mFOV;
    float mNearClip;
    float mFarClip;
};
