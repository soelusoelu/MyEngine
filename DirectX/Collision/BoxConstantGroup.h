﻿#pragma once

//ボックス定数群
namespace BoxConstantGroup {
//ボックスの頂点数
constexpr int POINTS_NUM = 8;
//ボックスの面の数
constexpr int SURFACES_NUM = 6;

//ボックスの各頂点の番号
constexpr int BOX_NEAR_BOTTOM_LEFT = 0;
constexpr int BOX_NEAR_BOTTOM_RIGHT = 1;
constexpr int BOX_BACK_BOTTOM_LEFT = 2;
constexpr int BOX_BACK_BOTTOM_RIGHT = 3;
constexpr int BOX_NEAR_TOP_LEFT = 4;
constexpr int BOX_NEAR_TOP_RIGHT = 5;
constexpr int BOX_BACK_TOP_LEFT = 6;
constexpr int BOX_BACK_TOP_RIGHT = 7;
}