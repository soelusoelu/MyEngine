﻿#pragma once

#include "../Math/Math.h"
#include <string>
#include <vector>

//ボーン構造体
struct Bone {
    //ボーン番号
    unsigned short number;
    //ボーン名
    std::string name;
    //初期姿勢
    Matrix4 initMat;
    //オフセット行列
    Matrix4 offsetMat;

    //親ノード
    const Bone* parent;
    //小ボーンたち
    std::vector<Bone*> children;
};
