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

    //ファイル拡張子からローダーを生成
    auto loader = createLoaderFromFilePath(filePath);
    if (!loader) {
        return nullptr;
    }

    mDirectory.setSoundDirectory(filePath);
    auto fileName = FileUtil::getFileNameFromDirectry(filePath);
    //音を読み込む
    WAVEFORMATEX format;
    auto res = loader->loadFromFile(&format, fileName);
    if (!res) {
        return nullptr;
    }

    //ソースボイス生成
    return mSoundBase->getXAudio2().createSourceVoice(mSoundBase->getMasteringVoice(), loader, format, param);
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
