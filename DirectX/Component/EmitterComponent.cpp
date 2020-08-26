#include "EmitterComponent.h"
#include "Sound/SoundComponent.h"
#include "../Device/Time.h"
#include "../Input/Input.h"
#include "../Sound/Player/SoundPlayer.h"
#include "../Transform/Transform3D.h"

EmitterComponent::EmitterComponent(GameObject& gameObject) :
    Component(gameObject),
    mSound(nullptr)
{
}

EmitterComponent::~EmitterComponent() = default;

void EmitterComponent::start() {
    mSound = getComponent<SoundComponent>();
    mSound->getSoundPlayer().playStreamingFadeIn(1.f, 2.f);
}

void EmitterComponent::update() {
    auto v = Input::keyboard().vertical();
    auto h = Input::keyboard().horizontal();
    transform().translate(Vector3(h, 0.f, v) * 10.f * Time::deltaTime);
}
