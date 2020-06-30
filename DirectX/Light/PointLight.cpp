#include "PointLight.h"
#include "../Device/AssetsManager.h"
#include "../DirectX/InputElement.h"
#include "../Mesh/IMeshLoader.h"
#include "../Mesh/Material.h"
#include "../System/Shader.h"
#include "../Utility/LevelLoader.h"

PointLight::PointLight() :
    shader(nullptr),
    radius(0.f),
    mMeshFileName("") {
}

PointLight::~PointLight() = default;

void PointLight::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["light"];
    if (obj.IsObject()) {
        JsonHelper::getString(obj, "pointLightMeshFileName", &mMeshFileName);
    }
}

void PointLight::initialize() {
    mesh = Singleton<AssetsManager>::instance().createMesh(mMeshFileName);
    mesh->setInitMaterials(&materials);
    radius = mesh->getRadius();
    shader = Singleton<AssetsManager>::instance().createShader("PointLight.hlsl");

    //コンスタントバッファ生成
    shader->createConstantBuffer(sizeof(PointLightConstantBuffer));

    //インプットレイアウトの生成
    static constexpr InputElementDesc layout[] = {
        { "POSITION", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, 0, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "NORMAL", 0, VertexType::VERTEX_TYPE_FLOAT3, 0, sizeof(float) * 3, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, VertexType::VERTEX_TYPE_FLOAT2, 0, sizeof(float) * 6, SlotClass::SLOT_CLASS_VERTEX_DATA, 0 },
    };
    static constexpr unsigned numElements = sizeof(layout) / sizeof(layout[0]);
    shader->createInputLayout(layout, numElements);
}
