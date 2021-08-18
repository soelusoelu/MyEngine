#include "OctopusFootManager.h"
#include "OctopusConstantNumbers.h"
#include "OctopusFoot.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Utility/StringUtil.h"
#include <algorithm>
#include <cassert>

OctopusFootManager::OctopusFootManager()
    : Component()
    , mFootAliveNumbers(OctopusConstantNumbers::FOOT_COUNT)
{
}

OctopusFootManager::~OctopusFootManager() = default;

void OctopusFootManager::start() {
    mFoots = getComponents<OctopusFoot>();
    assert(mFoots.size() == OctopusConstantNumbers::FOOT_COUNT);

    //全足共通処理
    for (unsigned i = 0; i < OctopusConstantNumbers::FOOT_COUNT; ++i) {
        auto& foot = mFoots[i];
        foot->onDestroyFoot([&](const OctopusFoot& foot) { onDestroyFoot(foot); });

        mFootAliveNumbers[i] = foot->getNumber();
    }
}

const OctopusFootPtrArray& OctopusFootManager::getFoots() const {
    return mFoots;
}

const AliveNumbers& OctopusFootManager::getAliveFootNumbers() const {
    return mFootAliveNumbers;
}

bool OctopusFootManager::isFootAlive() const {
    return (mFootAliveNumbers.size() > 0);
}

void OctopusFootManager::onDestroyFoot(const OctopusFoot& foot) {
    //死亡した足の番号を除外する
    auto itr = std::find(mFootAliveNumbers.begin(), mFootAliveNumbers.end(), foot.getNumber());
    if (itr != mFootAliveNumbers.end()) {
        mFootAliveNumbers.erase(itr);
    } else {
        //もし見つからなければエラー発行
        Debug::logError("Not found foot number. [" + StringUtil::intToString(foot.getNumber()) + "]");
    }
}
