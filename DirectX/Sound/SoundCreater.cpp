#include "SoundCreater.h"
#include "SoundBase.h"
#include "SourceVoice.h"
#include "WAV.h"
#include "XAudio2.h"
#include "../DebugLayer/Debug.h"
#include "../Utility/FileUtil.h"
#include <cassert>

SoundCreater::SoundCreater() :
    mSoundBase(std::make_unique<SoundBase>()) {
    assert(!mInstantiated);
    mInstantiated = true;
}

SoundCreater::~SoundCreater() {
    mInstantiated = false;
}

std::unique_ptr<SourceVoice> SoundCreater::create(const std::string& filePath, const SourceVoiceInitParam& param) {
    //サウンドAPIが使用できない状態ならnullptrを返す
    if (mSoundBase->isNull()) {
        return nullptr;
    }

    std::shared_ptr<WaveformData> data;
    auto itr = mSounds.find(filePath);
    if (itr != mSounds.end()) { //既に読み込まれている
        data = itr->second;
    } else { //初読み込み
        loadSound(&data, filePath, param);
        mSounds.emplace(filePath, data);
    }

    return mSoundBase->getXAudio2().createSourceVoice(*data, param);
}

void SoundCreater::loadSound(std::shared_ptr<WaveformData>* data, const std::string& filePath, const SourceVoiceInitParam& param) {
    auto loader = createLoaderFromFilePath(filePath);
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
