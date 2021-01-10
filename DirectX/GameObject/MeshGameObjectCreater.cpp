﻿#include "MeshGameObjectCreater.h"
#include "../Component/Engine/Mesh/MeshComponent.h"
#include "../Component/Engine/Mesh/MeshRenderer.h"
#include "../GameObject/GameObject.h"
#include "../Utility/FileUtil.h"

std::shared_ptr<GameObject> MeshGameObjectCreater::createMeshGameObject(
    const std::string& meshFilePath,
    const std::string& name,
    const std::string& tag
) {
    //ファイルパスからファイル名とディレクトリパスを抜き出す
    const auto& fileName = FileUtil::getFileNameFromDirectry(meshFilePath);
    const auto& directoryPath = FileUtil::getDirectryFromFilePath(meshFilePath);

    //ゲームオブジェクト生成
    auto newGameObject = GameObject::create(name, tag);
    //ゲームオブジェクトにメッシュをアタッチする
    auto newMesh = Component::addComponent<MeshComponent>(*newGameObject, "MeshComponent");
    //メッシュを生成する
    newMesh->createMesh(fileName, directoryPath);

    return newGameObject;
}

std::shared_ptr<GameObject> MeshGameObjectCreater::createMeshGameObject(
    IGameObjectAdder* gameObjectAdder,
    IMeshAdder* meshAdder,
    bool handleShadow,
    const std::string& meshFilePath,
    const std::string& name,
    const std::string& tag
) {
    //ファイルパスからファイル名とディレクトリパスを抜き出す
    const auto& fileName = FileUtil::getFileNameFromDirectry(meshFilePath);
    const auto& directoryPath = FileUtil::getDirectryFromFilePath(meshFilePath);

    //ゲームオブジェクト生成
    auto newGameObject = std::make_shared<GameObject>();
    newGameObject->setName(name);
    newGameObject->setTag(tag);
    //ゲームオブジェクトマネージャーに登録する
    gameObjectAdder->add(newGameObject);

    //ゲームオブジェクトにメッシュをアタッチする
    auto newMesh = Component::addComponent<MeshComponent>(*newGameObject, "MeshComponent");
    //メッシュを生成する
    newMesh->createMesh(fileName, directoryPath);
    //メッシュマネージャーに登録する
    meshAdder->add(newMesh->getComponent<MeshRenderer>(), handleShadow);

    return newGameObject;
}
