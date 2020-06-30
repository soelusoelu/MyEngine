#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class SoundComponent;

class Title : public Component {
public:
    Title();
    ~Title();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<SoundComponent> mSound;
};
