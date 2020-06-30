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
    void setTextureDirectory();
    void setSoundDirectory();
    void setDataDirectory();
    void setModelDirectory(const std::string& directry);

private:
    Directory(const Directory&) = delete;
    Directory& operator=(const Directory&) = delete;

    void setCurrentDirectory(const std::string& path);

private:
    std::string mRootPath;

    static inline bool mInstantiated = false;
};
