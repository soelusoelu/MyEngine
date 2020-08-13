#include "EmitterComponent.h"
#include "Sound/SoundComponent.h"
#include "../Device/Time.h"
#include "../Input/Input.h"
#include "../Sound/Player/SoundPlayer.h"
#include "../Transform/Transform3D.h"

EmitterComponent::EmitterComponent() :
    Component(),
    mSound(nullptr)
{
}

EmitterComponent::~EmitterComponent() = default;

void EmitterComponent::update() {
    auto v = Input::keyboard().vertical();
    auto h = Input::keyboard().horizontal();
    transform().translate(Vector3(h, 0.f, v) * 10.f * Time::deltaTime);
}

void EmitterComponent::start() {
    mSound = getComponent<SoundComponent>();
    mSound->getSoundPlayer().playStreamingFadeIn(1.f, 2.f);
}
