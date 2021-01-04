#include "SceneMeshOperator.h"
#include "SceneMeshDeleter.h"
#include "SceneMeshSelector.h"

SceneMeshOperator::SceneMeshOperator()
    : mSelector(std::make_unique<SceneMeshSelector>())
    , mDeleter(std::make_unique<SceneMeshDeleter>())
    , mCurrentSelectMesh(nullptr)
{
}

SceneMeshOperator::~SceneMeshOperator() = default;

const MeshComponent& SceneMeshOperator::getCurrentSelectMesh() const {
    return *mCurrentSelectMesh;
}

bool SceneMeshOperator::selectedMesh() const {
    return (mCurrentSelectMesh) ? true : false;
}

void SceneMeshOperator::update() {
    mSelector->selectMesh(mCurrentSelectMesh);
    mDeleter->deleteMesh(mCurrentSelectMesh);
}
