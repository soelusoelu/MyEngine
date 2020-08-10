#pragma once

class SourceVoice;
class SoundPlayer;

//ループを扱うクラス
class SoundLoop {
public:
    SoundLoop(SourceVoice& sourceVoice, SoundPlayer& player);
    ~SoundLoop();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// ループ区間を指定する
    /// </summary>
    /// <param name="begin">ループ開始地点(単位: 秒数)</param>
    /// <param name="end">ループ折返し地点(単位: 秒数)</param>
    void setLoopPoint(float begin, float end);

    /// <summary>
    /// ループから抜ける
    /// </summary>
    void exitLoop();

    /// <summary>
    /// ループしている最中か
    /// </summary>
    /// <returns></returns>
    constexpr bool isLoop() const;

private:
    SoundLoop(const SoundLoop&) = delete;
    SoundLoop& operator=(const SoundLoop&) = delete;

private:
    SourceVoice& mSourceVoice;
    SoundPlayer& mPlayer;
    //ループ開始地点 sec
    float mLoopBegin;
    //ループ折返し地点 sec
    float mLoopEnd;
    //ループ中か
    bool mIsLoop;
};
