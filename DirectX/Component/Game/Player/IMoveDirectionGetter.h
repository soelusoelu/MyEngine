#pragma once

#include "../../../Math/Math.h"

class IMoveDirectionGetter {
public:
    virtual ~IMoveDirectionGetter() = default;
    //最後に入力された移動方向を取得する
    virtual const Vector3& getMoveDirectionInputedLast() const = 0;
};
