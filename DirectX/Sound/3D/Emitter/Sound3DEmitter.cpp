#include "Sound3DEmitter.h"
#include "../../../Device/Time.h"

Sound3DEmitter::Sound3DEmitter(const WaveFormat& format) :
    mEmitter(),
    mPreviousPos(Vector3::zero),
    mIsCalculateLPFDirect(true),
    mIsCalculateLPFReverb(false),
    mIsCalculateReverb(false)
{
    const auto inCh = format.channels;

    mEmitter.cone = &EMITTER_CONE;
    mEmitter.channelCount = inCh;
    mEmitter.channelRadius = 1.f;
    mAzimuths.resize(inCh);
    mEmitter.channelAzimuths = mAzimuths.data();
    mEmitter.innerRadius = 2.f;
    mEmitter.innerRadiusAngle = Math::PI / 4.f;
    mEmitter.volumeCurve = &DEFAULT_LINEAR_CURVE;
    mEmitter.lfeCurve = &EMITTER_LFE_CURVE;
    mEmitter.lpfDirectCurve = nullptr; //デフォルトカーブを使用
    mEmitter.lpfReverbCurve = nullptr; //デフォルトカーブを使用
    mEmitter.reverbCurve = &EMITTER_REVERB_CURVE;
    mEmitter.curveDistanceScaler = 14.f;
    mEmitter.dopplerScaler = 1.f;
}

Sound3DEmitter::~Sound3DEmitter() = default;

void Sound3DEmitter::update() {
    mEmitter.velocity = (mEmitter.position - mPreviousPos) / Time::deltaTime;
    mPreviousPos = mEmitter.position;
}

void Sound3DEmitter::setPosition(const Vector3& pos) {
    mEmitter.position = pos;
}

void Sound3DEmitter::setDopplerScale(float scale) {
    scale = Math::clamp<float>(scale, 0.f, Math::infinity);
    mEmitter.dopplerScaler = scale;
}

bool Sound3DEmitter::isCalculateDoppler() const {
    return !Math::nearZero(mEmitter.dopplerScaler);
}

void Sound3DEmitter::setCalculateLPFDirect(bool value) {
    mIsCalculateLPFDirect = value;
}

bool Sound3DEmitter::isCalculateLPFDirect() const {
    return mIsCalculateLPFDirect;
}

void Sound3DEmitter::setCalculateLPFReverb(bool value) {
    mIsCalculateLPFReverb = value;
}

bool Sound3DEmitter::isCalculateLPFReverb() const {
    return mIsCalculateLPFReverb;
}

void Sound3DEmitter::setCalculateReverb(bool value) {
    mIsCalculateReverb = value;
}

bool Sound3DEmitter::isCalculateReverb() const {
    return mIsCalculateReverb;
}

const Sound3DEmitterParam& Sound3DEmitter::getEmitter() const {
    return mEmitter;
}
