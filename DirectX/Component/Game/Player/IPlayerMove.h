#pragma once

class IPlayerMove {
public:
    virtual ~IPlayerMove() = default;
    //moveSpeedに応じた速さで移動する
    virtual void move(float moveSpeed) = 0;
    //移動方向を向くように回転する
    virtual void rotateToMoveDirection() = 0;
};
