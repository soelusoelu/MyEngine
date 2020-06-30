#pragma once

#include <memory>

class Directory;
class AssetsManager;

//グローバルアクセス専用クラス
class World {
public:
    ~World();
    static World& instance();
    void finalize();
    Directory& directory() const;
    AssetsManager& assetsManager() const;

private:
    World();
    World(const World&) = delete;
    World& operator=(const World&) = delete;

private:
    static inline World* mInstance = nullptr;
    std::unique_ptr<Directory> mDirectory;
    std::unique_ptr<AssetsManager> mAssetsManager;
};
