#pragma once

#include <string>

class IAssetsRenderTextureList {
public:
    ~IAssetsRenderTextureList() = default;
    virtual void add(const std::string& fileName, const std::string& directoryPath) = 0;
};