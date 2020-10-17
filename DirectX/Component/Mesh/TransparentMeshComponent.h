#pragma once

#include "MeshComponent.h"
#include "../../Math/Math.h"
#include "../../System/GlobalFunction.h"
#include <string>

class DirectionalLight;

class TransparentMeshComponent : public MeshComponent {
public:
    TransparentMeshComponent(GameObject& gameObject);
    ~TransparentMeshComponent();
    virtual void awake() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    virtual void draw(const Camera& camera) const override;

    void setAlpha(float alpha);
    float getAlpha() const;

private:
    std::shared_ptr<DirectionalLight> mDirLight;
    float mAlpha;
};
