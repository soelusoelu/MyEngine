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
        auto loader = createLoaderFromFilePath(filePath);
        if (!loader) {
            return nullptr;
        }
        mDirectory.setSoundDirectory(filePath);
        auto fileName = FileUtil::getFileNameFromDirectry(filePath);
        //音を読み込む
        data = loader->loadFromFile(fileName);
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

std::unique_ptr<ISoundLoader> SoundCreater::createLoaderFromFilePath(const std::string& filePath) {
    std::unique_ptr<ISoundLoader> loader = nullptr;
    auto ext = FileUtil::getFileExtension(filePath);
    if (ext == ".wav") {
        loader = std::make_unique<WAV>();
    } else {
        Debug::windowMessage(filePath + ": 対応していない拡張子です");
    }

    return loader;
}
