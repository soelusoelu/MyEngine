#pragma once

#include "../Math/Math.h"
#include "../System/GlobalFunction.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>
#include <vector>

class IMeshLoader;
class Shader;
struct Material;

struct PointLightConstantBuffer {
    ALIGN16 Matrix4 wvp;
    ALIGN16 Vector3 worldPos;
    ALIGN16 Vector3 cameraPos;
    ALIGN16 Vector2 windowSize;
    ALIGN16 Vector3 diffuseColor;
    ALIGN16 float innerRadius;
    ALIGN16 float outerRadius;
    ALIGN16 float intensity;
};

struct PointLight {
    std::shared_ptr<IMeshLoader> mesh;
    std::vector<std::shared_ptr<Material>> materials;
    std::shared_ptr<Shader> shader;
    float radius;

    PointLight();
    ~PointLight();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();

private:
    std::string mMeshFileName;
};
