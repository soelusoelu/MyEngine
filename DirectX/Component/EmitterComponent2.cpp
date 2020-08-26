#include "EmitterComponent2.h"
#include "Sound/SoundComponent.h"
#include "../Input/Input.h"
#include "../Sound/Player/SoundPlayer.h"

EmitterComponent2::EmitterComponent2(GameObject& gameObject) :
    Component(gameObject),
    mSound(nullptr) {
}

EmitterComponent2::~EmitterComponent2() = default;

void EmitterComponent2::start() {
    mSound = getComponent<SoundComponent>();
}

void EmitterComponent2::update() {
    if (!mSound->getSoundPlayer().isStop()) {
        return;
    }
    if (Input::keyboard().getKeyDown(KeyCode::Space)) {
        mSound->getSoundPlayer().playStreamingFadeIn(1.f, 0.25f);
    }
}
