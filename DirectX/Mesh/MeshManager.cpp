#include "MeshManager.h"
#include "../Component/Camera/Camera.h"
#include "../Component/Mesh/MeshComponent.h"
#include "../DirectX/BlendDesc.h"
#include "../DirectX/BlendState.h"
#include "../DirectX/DepthStencilState.h"
#include "../DirectX/DirectX.h"
#include "../DirectX/RasterizerDesc.h"
#include "../DirectX/RasterizerState.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"

MeshManager::MeshManager() = default;
MeshManager::~MeshManager() = default;

void MeshManager::update() {
    remove();
}

void MeshManager::draw(const Camera& camera) const {
    if (mMeshes.empty()) {
        return;
    }

    auto& dx = Singleton<DirectX>::instance();
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);

    for (const auto& mesh : mMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        RasterizerDesc rd;

        rd.cullMode = CullMode::FRONT;
        dx.rasterizerState()->setRasterizerState(rd);
        mesh->draw();

        rd.cullMode = CullMode::BACK;
        dx.rasterizerState()->setRasterizerState(rd);
        mesh->draw();
    }
}

void MeshManager::drawTransparent(const Camera& camera) const {
    if (mTransparentMeshes.empty()) {
        return;
    }

    auto& dx = Singleton<DirectX>::instance();
    //プリミティブ・トポロジーをセット
    dx.setPrimitive(PrimitiveType::PRIMITIVE_TYPE_TRIANGLE_LIST);
    //デプステスト有効化
    dx.depthStencilState()->depthTest(true);
    //デプスマスク有効化
    dx.depthStencilState()->depthMask(true);
    //通常合成
    BlendDesc bd;
    bd.renderTarget.srcBlend = Blend::SRC_ALPHA;
    bd.renderTarget.destBlend = Blend::INV_SRC_ALPHA;
    dx.blendState()->setBlendState(bd);

    for (const auto& mesh : mTransparentMeshes) {
        if (!isDraw(*mesh, camera)) {
            continue;
        }

        RasterizerDesc rd;

        rd.cullMode = CullMode::FRONT;
        dx.rasterizerState()->setRasterizerState(rd);
        mesh->draw();

        rd.cullMode = CullMode::BACK;
        dx.rasterizerState()->setRasterizerState(rd);
        mesh->draw();
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
    auto pos = mesh.gameObject()->transform()->getPosition();
    auto radius = mesh.getRadius() * mesh.gameObject()->transform()->getScale().x;
    if (!camera.viewFrustumCulling(pos, radius)) {
        return false;
    }

    return true;
}
