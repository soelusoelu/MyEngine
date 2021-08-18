#include "EnemyMove.h"
#include "EnemyAI.h"
#include "EnemyMotions.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Transform/Transform3D.h"
#include "../../../Utility/Easing.h"
#include "../../../Utility/JsonHelper.h"

EnemyMove::EnemyMove()
    : Component()
    , mPlayer(nullptr)
    , mAnimation(nullptr)
    , mMoveSpeed(0.f)
    , mRotateSpeed(0.f)
    , mAngleNeedsRotate(0.f)
{
}

EnemyMove::~EnemyMove() = default;

void EnemyMove::start() {
    mAnimation = getComponent<SkinMeshComponent>();

    getComponent<EnemyAI>()->onSetPlayer([&](const std::shared_ptr<GameObject>& player) { mPlayer = player; });
}

void EnemyMove::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mMoveSpeed, "moveSpeed", inObj, alloc, mode);
    JsonHelper::getSet(mRotateSpeed, "rotateSpeed", inObj, alloc, mode);
    JsonHelper::getSet(mAngleNeedsRotate, "angleNeedsRotate", inObj, alloc, mode);
}

void EnemyMove::drawInspector() {
    ImGuiWrapper::dragFloat("Move Speed", mMoveSpeed, 0.1f);
    ImGuiWrapper::dragFloat("Rotate Speed", mRotateSpeed, 0.1f);
    ImGuiWrapper::sliderFloat("Angle Needs Rotate", mAngleNeedsRotate, 0.f, 180.f);
}

void EnemyMove::move() {
    transform().translate(getToPlayer() * mMoveSpeed * Time::deltaTime);
}

void EnemyMove::rotate() {
    auto f = mRotateSpeed * Time::deltaTime;
    f = Easing::easeInCubic(f);

    auto& t = transform();
    auto lookRot = Quaternion::lookRotation(getToPlayer());
    t.setRotation(Quaternion::slerp(t.getRotation(), lookRot, f));
}

bool EnemyMove::shouldRotate() const {
    //Tポーズ中のみ回転可能
    if (!isCurrentMotionTPose()) {
        return false;
    }

    auto dot = Vector3::dot(transform().forward(), getToPlayer());
    auto angle = Math::acos(dot);

    return (angle * 2.f > mAngleNeedsRotate);
}

bool EnemyMove::isCurrentMotionTPose() const {
    return (mAnimation->getCurrentMotionNumber() == SkinMeshComponent::T_POSE_NO);
}

Vector3 EnemyMove::getToPlayer() const {
    return Vector3::normalize(mPlayer->transform().getPosition() - transform().getPosition());
}
