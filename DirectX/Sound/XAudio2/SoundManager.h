#pragma once

#include <list>
#include <memory>

class SourceVoice;
class MasteringVoice;
class Sound3DCalculator;
class Sound3DListener;

class SoundManager {
    using SoundPtr = std::shared_ptr<SourceVoice>;
    using SoundPtrList = std::list<SoundPtr>;

public:
    SoundManager(const MasteringVoice& masteringVoice);
    ~SoundManager();
    void update();
    void add(const SoundPtr& sound);
    void setListener(const std::shared_ptr<Sound3DListener>& listener);

private:
    SoundPtrList mSounds;
    std::unique_ptr<Sound3DCalculator> mCalculator;
    std::shared_ptr<Sound3DListener> mListener;
};
