#pragma once

#include "../../Component.h"
#include "../../../Device/Function.h"
#include <functional>
#include <memory>

class PlayerUIManager
    : public Component
{
    using Player = std::shared_ptr<GameObject>;

public:
    PlayerUIManager();
    ~PlayerUIManager();
    void setPlayer(const Player& player);
    void callbackSetPlayer(const std::function<void(const GameObject&)>& callback);

private:
    PlayerUIManager(const PlayerUIManager&) = delete;
    PlayerUIManager& operator=(const PlayerUIManager&) = delete;

public:
    static constexpr unsigned SPRITE_HP = 0;
    static constexpr unsigned SPRITE_STAMINA = 1;

private:
    Function<void(const GameObject&)> mCallbackSetPlayer;
};
