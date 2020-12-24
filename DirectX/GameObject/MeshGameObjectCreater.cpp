#include "MeshGameObjectCreater.h"
#include "../Component/Engine/Mesh/MeshComponent.h"
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
