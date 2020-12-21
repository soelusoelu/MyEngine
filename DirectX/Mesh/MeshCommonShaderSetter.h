#pragma once

#include "Material.h"
#include "../System/Shader/ConstantBuffers.h"

class Camera;
class DirectionalLight;
class Transform3D;

//メッシュ共通定数バッファを設定するクラス
class MeshCommonShaderSetter {
public:
    static void setCommon(
        MeshCommonConstantBuffer& out,
        const Camera& camera,
        const DirectionalLight& dirLight,
        const Transform3D& transform
    );

    static void setMaterial(
        MaterialConstantBuffer& out,
        const Material& material,
        bool transferTexture = true
    );

private:
    MeshCommonShaderSetter() = delete;
    ~MeshCommonShaderSetter() = delete;
};
