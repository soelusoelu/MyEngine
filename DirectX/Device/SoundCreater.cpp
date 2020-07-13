#include "SoundCreater.h"
#include "../DebugLayer/Debug.h"
#include "../Sound/Loader/WAV.h"
#include "../Sound/Voice/SourceVoice/SourceVoice.h"
#include "../Sound/Voice/SubmixVoice/SubmixVoice.h"
#include "../Sound/XAudio2/SoundBase.h"
#include "../Sound/XAudio2/XAudio2.h"
#include "../Utility/Directory.h"
#include "../Utility/FileUtil.h"
#include <cassert>

SoundCreater::SoundCreater(Directory& directory) :
    mDirectory(directory),
    mSoundBase(std::make_unique<SoundBase>()) {
    assert(!mInstantiated);
    mInstantiated = true;
}

SoundCreater::~SoundCreater() {
    mInstantiated = false;
}

std::unique_ptr<SourceVoice> SoundCreater::createSourceVoice(const std::string& filePath, const SourceVoiceInitParam& param) {
    //サウンドAPIが使用できない状態ならnullptrを返す
    if (mSoundBase->isNull()) {
        return nullptr;
    }

    std::shared_ptr<WaveformData> data = nullptr;
    auto itr = mSounds.find(filePath);
    if (itr != mSounds.end()) { //既に読み込まれている
        data = itr->second;
    } else { //初読み込み
        loadSound(&data, filePath, param);
        //データの読み込みに失敗していたら
        if (!data) {
            return nullptr;
        }
        mSounds.emplace(filePath, data);
    }

    return mSoundBase->getXAudio2().createSourceVoice(mSoundBase->getMasteringVoice(), *data, param);
}

std::unique_ptr<SubmixVoice> SoundCreater::createSubmixVoice(const SubmixVoiceInitParam& param) const {
    //サウンドAPIが使用できない状態ならnullptrを返す
    if (mSoundBase->isNull()) {
        return nullptr;
    }

    return mSoundBase->getXAudio2().createSubmixVoice(mSoundBase->getMasteringVoice(), param);
}

void SoundCreater::loadSound(std::shared_ptr<WaveformData>* data, const std::string& filePath, const SourceVoiceInitParam& param) {
    auto loader = createLoaderFromFilePath(filePath);
    if (!loader) {
        return;
    }

    mDirectory.setSoundDirectory(filePath);
    *data = std::make_shared<WaveformData>();
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    loader->loadFromFile(data, fileName);
}

std::shared_ptr<ISoundLoader> SoundCreater::createLoaderFromFilePath(const std::string& filePath) {
    std::shared_ptr<ISoundLoader> loader = nullptr;
    auto ext = FileUtil::getFileExtension(filePath);
    if (ext == ".wav") {
        loader = std::make_shared<WAV>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
    }

    return loader;
}
