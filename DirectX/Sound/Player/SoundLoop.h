#pragma once

#include "../Voice/VoiceDetails.h"

class SoundPlayer;
class SoundPlayTimer;

//ループを扱うクラス
class SoundLoop {
public:
    SoundLoop(SoundPlayer& player, SoundPlayTimer& playTimer, const SoundData& data);
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
    SoundPlayer& mPlayer;
    SoundPlayTimer& mPlayTimer;
    const SoundData& mData;
    //ループ開始地点 sec
    float mLoopBegin;
    //ループ折返し地点 sec
    float mLoopEnd;
    //ループ中か
    bool mIsLoop;
};
