#pragma once

#include "../Component.h"
#include "../../Math/Math.h"
#include "../../Mesh/Material.h"
#include <memory>
#include <string>
#include <vector>

class Mesh;
class MeshManager;
class Camera;

class MeshComponent : public Component, public std::enable_shared_from_this<MeshComponent> {
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

public:
    MeshComponent(GameObject& gameObject);
    virtual ~MeshComponent();
    virtual void awake() override;
    virtual void onEnable(bool value) override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    virtual void draw(const Camera& camera) const = 0;

    //中心座標の取得
    const Vector3& getCenter() const;
    //半径の取得
    float getRadius() const;
    //状態
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

    //自身を管理するマネージャーを登録する
    static void setMeshManager(MeshManager* manager);

private:
    void addToManager();

protected:
    std::unique_ptr<Mesh> mMesh;
    std::string mFileName;
    State mState;

    static inline MeshManager* mMeshManager = nullptr;
};
