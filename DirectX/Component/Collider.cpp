﻿#include "Collider.h"
#include "../Device/Physics.h"
#include <algorithm>

Collider::Collider() :
    Component(),
    mIsAutoUpdate(true),
    mEnable(false) {
}

Collider::~Collider() = default;

void Collider::start() {
    if (mPhysics) {
        mPhysics->add(shared_from_this());
        mEnable = true;
    }
}

void Collider::lateUpdate() {
    mPreviousCollider.resize(mCurrentCollider.size());
    std::copy(mCurrentCollider.begin(), mCurrentCollider.end(), mPreviousCollider.begin());
    mCurrentCollider.clear();
}

void Collider::finalize() {
    mPreviousCollider.clear();
    mCurrentCollider.clear();

    if (mPhysics) {
        mPhysics->remove(shared_from_this());
    }
}

void Collider::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("IsAutoUpdate", mIsAutoUpdate);
    inspect->emplace_back("Enable", mEnable);
}

void Collider::onSetActive(bool value) {
    (value) ? enabled() : disabled();
}

void Collider::enabled() {
    mEnable = true;
}

void Collider::disabled() {
    mEnable = false;
}

bool Collider::getEnable() const {
    return mEnable;
}

void Collider::automation() {
    if (!mIsAutoUpdate) {
        mIsAutoUpdate = true;
    }
}

void Collider::addHitCollider(const CollPtr& hit) {
    mCurrentCollider.emplace_back(hit);
}

std::list<std::shared_ptr<Collider>> Collider::onCollisionEnter() {
    std::list<std::shared_ptr<Collider>> temp;
    for (const auto& c : mCurrentCollider) {
        auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), c);
        if (itr == mPreviousCollider.end()) {
            temp.emplace_back(c);
        }
    }

    return temp;
}

std::list<std::shared_ptr<Collider>> Collider::onCollisionStay() {
    std::list<std::shared_ptr<Collider>> temp;
    for (const auto& c : mCurrentCollider) {
        auto itr = std::find(mPreviousCollider.begin(), mPreviousCollider.end(), c);
        if (itr != mPreviousCollider.end()) {
            temp.emplace_back(c);
        }
    }

    return temp;
}

std::list<std::shared_ptr<Collider>> Collider::onCollisionExit() {
    std::list<std::shared_ptr<Collider>> temp;
    for (const auto& c : mPreviousCollider) {
        auto itr = std::find(mCurrentCollider.begin(), mCurrentCollider.end(), c);
        if (itr == mCurrentCollider.end()) {
            temp.emplace_back(c);
        }
    }

    return temp;
}

void Collider::setPhysics(Physics* physics) {
    mPhysics = physics;
}

Physics* Collider::mPhysics = nullptr;
