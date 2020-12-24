#pragma once

#include <memory>
#include <string>

class GameObject;

//メッシュ付きゲームオブジェクトを生成するクラス
class MeshGameObjectCreater {
public:
    //メッシュ付きゲームオブジェクトを生成する
    static std::shared_ptr<GameObject> createMeshGameObject(
        const std::string& meshFilePath,
        const std::string& name,
        const std::string& tag = "None"
    );
};
