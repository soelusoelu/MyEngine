#pragma once

#include "Listener/Sound3DListenerParam.h"
#include "../../Device/Flag.h"
#include <xaudio2.h>
#include <x3daudio.h>

class MasteringVoice;
class Sound3DEmitter;

//3D�T�E���h�v�Z�N���X
class Sound3DCalculator {
public:
    Sound3DCalculator(const MasteringVoice& masteringVoice);
    ~Sound3DCalculator();

    /// <summary>
    /// 3D�T�E���h���v�Z����
    /// </summary>
    /// <param name="listenerParam">���X�i�[�\����</param>
    /// <param name="emitter">�G�~�b�^�[</param>
    void calculate(const Sound3DListenerParam& listener, const Sound3DEmitter& emitter) const;

private:
    Sound3DCalculator(const Sound3DCalculator&) = delete;
    Sound3DCalculator& operator=(const Sound3DCalculator&) = delete;

    //�t���O��ݒ肵�Ԃ�
    Flag getFlagsFromEmitter(const Sound3DEmitter& emitter) const;

private:
    //3D�T�E���h�̃n���h��
    X3DAUDIO_HANDLE mInstance;
    //�������ɐ������Ă��邩
    bool mInitialized;
};
