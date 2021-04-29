#include "SkinMeshComponent.h"
#include "MeshComponent.h"
#include "MeshShader.h"
#include "../../../Device/Subject.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../System/Shader/ConstantBuffers.h"
#include <cassert>

SkinMeshComponent::SkinMeshComponent(GameObject& gameObject)
    : Component(gameObject)
    , mAnimation(nullptr)
    , mMeshShader(nullptr)
    , mCallbackComputeCurrentBones(std::make_unique<Subject>())
    , mCurrentMotionNo(0)
    , mCurrentFrame(0)
    , mIsMotionUpdate(true)
    , mIsLoop(true)
{
}

SkinMeshComponent::~SkinMeshComponent() = default;

void SkinMeshComponent::update() {
    if (mIsMotionUpdate) {
        calcNextPose();

        //通知を送る
        mCallbackComputeCurrentBones->notify();
    }

    //gpu側でスキニングする場合に送る必要がある
    //mMeshShader->setTransferData(mCurrentBones.data(), sizeof(SkinMeshConstantBuffer), 3);
}

void SkinMeshComponent::setMotionName(const std::string& name, unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mAnimation->setMotionName(name, motionNo);
}

void SkinMeshComponent::changeMotion(unsigned motionNo) {
    assert(motionNo < mAnimation->getMotionCount());
    mCurrentMotionNo = motionNo;
    mCurrentFrame = 0;
    mIsMotionUpdate = true;
}

void SkinMeshComponent::changeMotion(const std::string& motionName) {
    for (size_t i = 0; i < mAnimation->getMotionCount(); ++i) {
        if (mAnimation->getMotion(i).name == motionName) {
            mCurrentMotionNo = i;
            mCurrentFrame = 0;
            mIsMotionUpdate = true;
            return;
        }
    }

    Debug::logError("Not found motion name[" + motionName + "].");
}

void SkinMeshComponent::tPose() {
    mIsMotionUpdate = false;
    mCurrentBones.assign(mCurrentBones.size(), Matrix4::identity);
}

void SkinMeshComponent::setMotionUpdateFlag(bool value) {
    mIsMotionUpdate = value;
}

bool SkinMeshComponent::getMotionUpdateFlag() const {
    return mIsMotionUpdate;
}

void SkinMeshComponent::setLoop(bool value) {
    mIsLoop = value;
}

bool SkinMeshComponent::getLoop() const {
    return mIsLoop;
}

int SkinMeshComponent::getMotionCount() const {
    return mAnimation->getMotionCount();
}

const Motion& SkinMeshComponent::getCurrentMotion() const {
    return mAnimation->getMotion(mCurrentMotionNo);
}

int SkinMeshComponent::getCurrentMotionNumber() const {
    return mCurrentMotionNo;
}

int SkinMeshComponent::getCurrentMotionFrame() const {
    return mCurrentFrame;
}

const std::vector<Matrix4>& SkinMeshComponent::getBoneCurrentFrameMatrix() const {
    return mCurrentBones;
}

void SkinMeshComponent::setValue(const std::shared_ptr<MeshShader>& meshShader, IAnimation* anim) {
    mAnimation = anim;
    mMeshShader = meshShader;

    mCurrentBones.resize(mAnimation->getBoneCount());
}

void SkinMeshComponent::callbackComputeCurrentBones(const std::function<void()>& callback) {
    mCallbackComputeCurrentBones->addObserver(callback);
}

void SkinMeshComponent::calcNextPose() {
    const auto& motion = mAnimation->getMotion(mCurrentMotionNo);

    //シェーダーにボーンのデータを渡す
    for (size_t i = 0; i < mAnimation->getBoneCount(); ++i) {
        mCurrentBones[i] = mAnimation->getBone(i).offsetMat * motion.frameMat[i][mCurrentFrame];
    }

    calcCurrentFrame();
}

void SkinMeshComponent::calcCurrentFrame() {
    const auto& motion = mAnimation->getMotion(mCurrentMotionNo);

    ++mCurrentFrame;

    if (mCurrentFrame < motion.numFrame) {
        return;
    }

    if (mIsLoop) {
        mCurrentFrame = 0;
    } else {
        mCurrentFrame = motion.numFrame - 1;
        mIsMotionUpdate = false;
    }
}
