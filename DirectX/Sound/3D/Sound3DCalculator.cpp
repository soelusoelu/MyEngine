#include "Sound3DCalculator.h"
#include "Emitter/Sound3DEmitter.h"
#include "Emitter/DspSetter.h"
#include "../Voice/MasteringVoice/MasteringVoice.h"
#include "../../DebugLayer/Debug.h"
#include "../../Math/Math.h"

//���O�`�F�b�N
static_assert(sizeof(Vector3) == sizeof(X3DAUDIO_VECTOR), "Vector3 size does not match.");
static_assert(sizeof(Sound3DListenerParam) == sizeof(X3DAUDIO_LISTENER), "Listener sizes do not match.");
static_assert(sizeof(Sound3DEmitterParam) == sizeof(X3DAUDIO_EMITTER), "Emitter sizes do not match.");

Sound3DCalculator::Sound3DCalculator(const MasteringVoice& masteringVoice) :
    mInstance(),
    mInitialized(false) {
    auto res = X3DAudioInitialize(masteringVoice.getChannelMask(), X3DAUDIO_SPEED_OF_SOUND, mInstance);
    if (FAILED(res)) {
        Debug::logError("Failed 3d sound initialized.");
        return;
    }

    mInitialized = true;
}

Sound3DCalculator::~Sound3DCalculator() = default;

void Sound3DCalculator::calculate(const Sound3DListenerParam& listener, const Sound3DEmitter& emitter) const {
    if (!mInitialized) {
        return;
    }

    X3DAUDIO_LISTENER l;
    memcpy(&l, &listener, sizeof(Sound3DListenerParam));
    X3DAUDIO_EMITTER e;
    memcpy(&e, &emitter.getEmitter(), sizeof(Sound3DEmitterParam));
    //�K�v�ɉ������t���O��ݒ肷��
    auto flags = getFlagsFromEmitter(emitter);
    //�{�v�Z
    X3DAudioCalculate(mInstance, &l, &e, flags.get(), &emitter.getDspSetter().getDspSetting());

    //�v�Z���ʂ��\�[�X�{�C�X�ɓK�p����
    emitter.getDspSetter().applyToSourceVoice(flags);
}

Flag Sound3DCalculator::getFlagsFromEmitter(const Sound3DEmitter& emitter) const {
    //�t���O������������
    Flag flags;

    //�s��v�Z�͐��
    flags.set(X3DAUDIO_CALCULATE_MATRIX);
    //���[�p�X�t�B���^���ڃp�X
    if (emitter.isCalculateLPFDirect()) {
        flags.set(X3DAUDIO_CALCULATE_LPF_DIRECT);
    }
    //���o�[�u
    if (emitter.isCalculateReverb()) {
        flags.set(X3DAUDIO_CALCULATE_REVERB);
    }
    //���[�p�X�t�B���^���o�[�u�p�X
    if (emitter.isCalculateLPFReverb()) {
        flags.set(X3DAUDIO_CALCULATE_LPF_REVERB);
    }
    //�h�b�v���[����
    if (emitter.isCalculateDoppler()) {
        flags.set(X3DAUDIO_CALCULATE_DOPPLER);
    }

    return flags;
}
