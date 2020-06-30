#pragma once

#include <string>

class Directory {
public:
    static void initialize();
    static void finalize();
    static void setRootDirectory();
    static void setAssetsDirectory();
    static void setShaderDirectory();
    static void setTextureDirectory();
    static void setSoundDirectory();
    static void setDataDirectory();
    static void setModelDirectory(const std::string& directry);

private:
    Directory() = delete;
    ~Directory() = delete;
    Directory(const Directory&) = delete;
    Directory& operator=(const Directory&) = delete;

    static void setCurrentDirectory(const std::string& path);

private:
    static inline std::string mRootPath = "";
};
