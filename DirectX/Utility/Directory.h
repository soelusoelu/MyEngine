#pragma once

#include <string>

class Directory {
public:
    Directory();
    ~Directory();
    void initialize();
    void setRootDirectory();
    void setAssetsDirectory();
    void setShaderDirectory();
    void setTextureDirectory(const std::string& filePath);
    void setSoundDirectory(const std::string& filePath);
    void setDataDirectory();
    void setModelDirectory(const std::string& filePath);

private:
    Directory(const Directory&) = delete;
    Directory& operator=(const Directory&) = delete;

    void setCurrentDirectory(const std::string& path);

private:
    std::string mRootPath;

    static inline bool mInstantiated = false;
};
