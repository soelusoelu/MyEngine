#pragma once

#include "../Voice/IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

//ボイスの出力先管理クラス
class OutputVoices {
public:
    OutputVoices(IVoice& voice);
    ~OutputVoices();

    /// <summary>
    /// 毎フレーム更新
    /// </summary>
    void update();

    /// <summary>
    /// 出力先ボイスを設定する
    /// </summary>
    /// <param name="voice">設定したいボイス</param>
    /// <param name="useFilter">フィルターを使用するか</param>
    void setOutputVoices(const std::vector<std::shared_ptr<IVoice>>& voices, bool useFilter = false);

    /// <summary>
    /// ボイスの出力先を追加する
    /// </summary>
    /// <param name="voice">設定したいボイス</param>
    /// <param name="useFilter">フィルターを使用するか</param>
    void addOutputVoice(IVoice& voice, bool useFilter = false);

    /// <summary>
    /// 添字に対応するディスクリプタを返す
    /// </summary>
    /// <param name="index">配列の添字</param>
    /// <returns></returns>
    const XAUDIO2_SEND_DESCRIPTOR& getDesc(unsigned index) const;

    /// <summary>
    /// ディスクリプタ配列のサイズを返す
    /// </summary>
    /// <returns></returns>
    size_t size() const;

    //設定を適用する
    void apply();

private:
    OutputVoices(const OutputVoices&) = delete;
    OutputVoices& operator=(const OutputVoices&) = delete;

private:
    IVoice& mVoice;
    std::vector<XAUDIO2_SEND_DESCRIPTOR> mDescs;
    bool mHasChangedDescs;
};
