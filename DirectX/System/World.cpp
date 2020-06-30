#include "World.h"
#include "../Device/AssetsManager.h"
#include "../System/GlobalFunction.h"
#include "../Utility/Directory.h"

World::World() :
    mDirectory(std::make_unique<Directory>()),
    mAssetsManager(std::make_unique<AssetsManager>()) {
}

World::~World() = default;

World& World::instance() {
    if (!mInstance) {
        mInstance = new World();
    }
    return *mInstance;
}

void World::finalize() {
    safeDelete(mInstance);
}

Directory& World::directory() const {
    return *mDirectory;
}

AssetsManager& World::assetsManager() const {
    return *mAssetsManager;
}
