#include "PlayerEnemyConnection.h"
#include "../Enemy/Octopus.h"
#include "../Enemy/OctopusFoot.h"
#include "../Enemy/OctopusFootManager.h"
#include "../Enemy/OctopusHead.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"

PlayerEnemyConnection::PlayerEnemyConnection()
    : Component()
    , mOctopus(nullptr)
    , mShotRay(nullptr)
{
}

PlayerEnemyConnection::~PlayerEnemyConnection() = default;

void PlayerEnemyConnection::lateUpdate() {
    if (mShotRay) {
        intersectRayOctopus();

        mShotRay = nullptr;
    }
}

void PlayerEnemyConnection::setShotRay(const Ray* ray) {
    mShotRay = ray;
}

void PlayerEnemyConnection::setEnemy(const GameObject& enemy) {
    mOctopus = enemy.componentManager().getComponent<Octopus>();
}

void PlayerEnemyConnection::intersectRayOctopus() const {
    if (mOctopus->getFootManager().isFootAlive()) {
        intersectRayOctopusFoots();
    } else {
        intersectRayOctopusHead();
    }
}

void PlayerEnemyConnection::intersectRayOctopusFoots() const {
    const auto& foots = mOctopus->getFootManager().getFoots();

    for (const auto& foot : foots) {
        intersectRayOctopusPart(foot.get());
    }
}

void PlayerEnemyConnection::intersectRayOctopusHead() const {
    intersectRayOctopusPart(&mOctopus->getHead());
}

void PlayerEnemyConnection::intersectRayOctopusPart(IOctopusPart* part) const {
    //部位が死んでたら終了
    if (part->isDestroy()) {
        return;
    }

    //部位の全当たり判定と衝突判定
    const auto& colliders = part->getColliders();
    for (const auto& c : colliders) {
        if (Intersect::intersectRayOBB(*mShotRay, c->getOBB())) {
            part->takeDamage();
            Debug::log("Hit");
            return;
        }
    }
}
