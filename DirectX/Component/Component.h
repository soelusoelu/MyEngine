﻿#pragma once

#include "../GameObject/Object.h"
#include <rapidjson/document.h>
#include <any>
#include <list>
#include <memory>
#include <string>
#include <utility>

class GameObject;
class Transform3D;

namespace ComponentDebug {
    using DebugInfo = std::pair<std::string, std::any>;
    using DebugInfoList = std::list<DebugInfo>;
}

class Component : public Object {
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    Component();
    virtual ~Component();
    //loadPropertiesの直後に呼び出される
    virtual void awake() {};
    //getComponentはここでして
    virtual void start() {};
    //毎フレーム呼ばれる
    virtual void update() {};
    //全ゲームオブジェクトの全アップデート後に呼ばれる
    virtual void lateUpdate() {};
    //終了処理
    virtual void finalize() {};
    //オーナーのTransformが更新されたら
    virtual void onUpdateWorldTransform() {};
    //アクティブ・非アクティブ時の切り替え
    virtual void onSetActive(bool value) {};
    //ロード/セーブ
    virtual void loadProperties(const rapidjson::Value& inObj) {};
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const {};
    //Inspectorに表示する情報
    //first: 変数名
    //second: 値
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {};

    //コンポーネントがアタッチされているゲームオブジェクトを返す
    GameObjectPtr gameObject() const;
    //トランスフォームの取得
    const std::shared_ptr<Transform3D>& transform() const;
    //コンポーネントの名前を返す
    const std::string& getComponentName() const;

    //指定されたプロパティでコンポーネントを生成
    template <typename T>
    static void create(const GameObjectPtr& gameObject, const std::string& componentName, const rapidjson::Value& inObj) {
        auto t = std::make_shared<T>();
        t->mGameObject = gameObject;
        t->mComponentName = componentName;
        t->gameObject()->componentManager()->addComponent(t);
        t->loadProperties(inObj);
        t->awake();
    }

private:
    std::weak_ptr<GameObject> mGameObject;
    std::string mComponentName;
};
