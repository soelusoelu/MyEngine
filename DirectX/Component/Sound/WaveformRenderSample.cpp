#include "WaveformRenderSample.h"
#include "../Sound/SoundComponent.h"
#include "../Sprite/SpriteComponent.h"
#include "../../DebugLayer/Debug.h"
#include "../../System/TextureFromMemory.h"
#include "../../Transform/Transform2D.h"
#include "../../Sound/Effects/SoundEffect.h"
#include "../../Sound/Effects/SoundEffectCollection.h"
#include "../../Sound/Effects/FourierTransform/DiscreteFourierTransform.h"
#include "../../Sound/Player/SoundPlayer.h"
#include "../../Input/Input.h"
#include "../../System/Window.h"
#include <complex>

WaveformRenderSample::WaveformRenderSample(GameObject& gameObject) :
    Component(gameObject),
    mTexture(nullptr),
    mSound(nullptr),
    mFourierID(0) {
}

WaveformRenderSample::~WaveformRenderSample() = default;

void WaveformRenderSample::start() {
    //auto sprite = getComponent<SpriteComponent>();
    //mTexture = std::make_shared<TextureFromMemory>(WIDTH, HEIGHT);
    //sprite->setTexture(mTexture);
    //sprite->transform().setScale(Vector2(
    //    static_cast<float>(Window::standardWidth()) / static_cast<float>(WIDTH),
    //    static_cast<float>(Window::standardHeight()) / static_cast<float>(HEIGHT)
    //));

    mSound = getComponent<SoundComponent>();
    mFourierID = mSound->getSoundEffect().getEffectCollection().discreteFourierTransform();
    mSound->getSoundPlayer().playStreamingFadeIn(0.25f, 1.f);
}

void WaveformRenderSample::update() {
    mSound->getSoundEffect().getEffectParameters(mFourierID, &mFourierData, sizeof(mFourierData));
    if (mFourierData.size() == 0) {
        return;
    }

    //mTexture->clear();
    //for (size_t i = 0; i < mFourierData.size() / 2 - 1; i++) {
    //    auto y = Math::clamp<unsigned>(fabsf(mFourierData[i].imag()) * 10.f + HEIGHT / 2, 0, HEIGHT - 1);
    //    mTexture->setPixel(i, y, ColorPalette::black);
    //}
    //mTexture->apply();

    auto pre = Vector2(0.f, Window::standardHeight() / 2.f);
    size_t loopEnd = mFourierData.size() / 2 - 1;
    auto incX = static_cast<float>(Window::standardWidth()) / static_cast<float>(loopEnd);
    for (size_t i = 0; i < loopEnd; i++) {
        //auto y = Math::clamp<unsigned>(Window::standardHeight() - fabsf(mFourierData[i].imag()) * 10.f, 0, Window::standardHeight());
        auto y = Math::clamp<unsigned>(mFourierData[i].imag() * 10.f + Window::standardHeight() / 2.f, 0, Window::standardHeight());

        auto next = Vector2(i * incX, y);
        Debug::renderLine2D(pre, next);
        pre = next;
    }
}
