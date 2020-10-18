#include "MeshManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../DirectX/DirectXInclude.h"
#include "../Transform/Transform3D.h"

MeshManager::MeshManager() {
    MeshComponent::setMeshManager(this);
}

MeshManager::~MeshManager() {
    MeshComponent::setMeshManager(nullptr);
}

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera) const {
    if (mMeshes.empty()) {
        return;
    }

    auto& dx = DirectX::instance();
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::TRIANGLE_LIST);

    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        //dx.rasterizerState()->setCulling(CullMode::FRONT);
        //mesh->draw(camera);

        dx.rasterizerState()->setCulling(CullMode::BACK);
        mesh->draw(camera);
    }
}

void MeshManager::drawTransparent(const Camera& camera) const {
    if (mTransparentMeshes.empty()) {
        return;
    }

    auto& dx = DirectX::instance();
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::TRIANGLE_LIST);
    //デプステスト有効化
    dx.depthStencilState()->depthTest(true);
    //デプスマスク有効化
    dx.depthStencilState()->depthMask(true);
    //半透明合成
    dx.blendState()->translucent();

    for (const auto& mesh : mTransparentMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        //dx.rasterizerState()->setCulling(CullMode::FRONT);
        //mesh->draw(camera);

        dx.rasterizerState()->setCulling(CullMode::BACK);
        mesh->draw(camera);
    }
}

void MeshManager::add(const MeshPtr& mesh) {
    mMeshes.emplace_back(mesh);
}

void MeshManager::addTransparent(const MeshPtr& mesh) {
    mTransparentMeshes.emplace_back(mesh);
}

void MeshManager::clear() {
    mMeshes.clear();
    mTransparentMeshes.clear();
}

void MeshManager::remove() {
    auto itr = mMeshes.begin();
    while (itr != mMeshes.end()) {
        if ((*itr)->isDead()) {
            itr = mMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
    itr = mTransparentMeshes.begin();
    while (itr != mTransparentMeshes.end()) {
        if ((*itr)->isDead()) {
            itr = mTransparentMeshes.erase(itr);
        } else {
            ++itr;
        }
    }
}

bool MeshManager::isDraw(const MeshComponent& mesh, const Camera& camera) const {
    if (!mesh.getActive()) {
        return false;
    }
    if (mesh.isDead()) {
        return false;
    }
    auto pos = mesh.transform().getPosition();
    auto radius = mesh.getRadius() * mesh.transform().getScale().x;
    if (!camera.viewFrustumCulling(pos, radius)) {
        return false;
    }

    return true;
}
