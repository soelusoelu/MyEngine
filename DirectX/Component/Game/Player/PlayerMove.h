#pragma once

#include "../../Component.h"

class PlayerMove
    : public Component
{
public:
    PlayerMove(GameObject& gameObject);
    ~PlayerMove();
    virtual void update() override;

private:
    PlayerMove(const PlayerMove&) = delete;
    PlayerMove& operator=(const PlayerMove&) = delete;
};
