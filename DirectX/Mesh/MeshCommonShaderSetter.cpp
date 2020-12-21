#include "MeshCommonShaderSetter.h"
#include "../Component/Engine/Camera/Camera.h"
#include "../Component/Engine/Light/DirectionalLight.h"
#include "../System/Texture/Texture.h"
#include "../Transform/Transform3D.h"

void MeshCommonShaderSetter::setCommon(
    MeshCommonConstantBuffer& out,
    const Camera& camera,
    const DirectionalLight& dirLight,
    const Transform3D& transform
) {
    const auto& world = transform.getWorldTransform();
    out.world = world;
    out.view = camera.getView();
    out.projection = camera.getProjection();
    out.wvp = world * camera.getViewProjection();
    out.lightDir = dirLight.getDirection();
    out.lightColor = dirLight.getLightColor();
    out.cameraPos = camera.getPosition();
}

void MeshCommonShaderSetter::setMaterial(
    MaterialConstantBuffer& out,
    const Material& material,
    bool transferTexture
) {
    out.ambient = material.ambient;
    out.diffuse = Vector4(material.diffuse, material.transparency);
    out.specular = material.specular;
    out.shininess = material.shininess;

    //テクスチャ登録
    if (transferTexture) {
        material.texture->setTextureInfo();
    }
}
