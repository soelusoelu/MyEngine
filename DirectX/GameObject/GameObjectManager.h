#pragma once

#include "IGameObjectsGetter.h"
#include <list>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class GameObjectManager : public IGameObjectsGetter {
    using GameObjectPtrArray = std::vector<GameObjectPtr>;

public:
    GameObjectManager();
    ~GameObjectManager();
    //全ゲームオブジェクトを取得する
    virtual const GameObjectPtrList& getGameObjects() const override;
    //登録済みの全ゲームオブジェクトの更新
    void update();
    //ゲームオブジェクトの登録
    void add(const GameObjectPtr& add);
    //登録済みの全ゲームオブジェクトの削除
    void clear(const std::unordered_set<std::string>& tags);
    //tagに一致するアクティブなゲームオブジェクトの検索
    const GameObjectPtr& find(const std::string& tag) const;
    //tagに一致するアクティブな全ゲームオブジェクトの検索
    GameObjectPtrArray findGameObjects(const std::string& tag) const;
    //ゲームオブジェクトの名前がかぶらないように番号で調整する
    void setNameNumber(std::string& name) const;

private:
    //コピー禁止
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    //ゲームオブジェクトの削除
    void remove();
    //ゲームオブジェクトの名前を走査していく
    void checkNameNumber(std::string& name, bool& isEnd, int number = 0) const;

private:
    //ゲームオブジェクトリスト
    GameObjectPtrList mGameObjects;
};
