#pragma once

#include "../Component.h"
#include "../../Sound/3D/Sound3DCone.h"
#include "../../Sound/3D/Listener/Sound3DListenerParam.h"
#include <memory>

class Sound3DListener;
class GameObject;

class ListenerComponent : public Component {
public:
    ListenerComponent();
    ~ListenerComponent();
    virtual void lateUpdate() override;
    //使用可能状態か
    bool isNull() const;
    //リスナーにしたいゲームオブジェクトを設定する
    void setTarget(const std::shared_ptr<GameObject>& target);
    //コーンを設定する
    void setCone(Sound3DCone* cone);
    //リスナー構造体を返す
    const Sound3DListenerParam& getListener() const;

private:
    std::unique_ptr<Sound3DListener> mListener;
    std::shared_ptr<GameObject> mTarget;
};
