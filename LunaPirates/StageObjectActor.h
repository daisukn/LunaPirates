#ifndef __OBJECTACTOR_H
#define __OBJECTACTOR_H

#include "Actor.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "ColliderComponent.h"
#include <memory>


// ゲームオブジェクトの基底クラス
class StageObjectActor : public Actor
{
public:
    StageObjectActor(class Application* a);

    void UpdateActor(float deltaTime) override;

    //void SetVisible(bool visible);
    //bool GetVisible() const { return isVisible; }
    
    bool GetDisp() const { return isDisp; }
    void SetDisp(bool b) { isDisp = b; }
    
    void SetOwnerStage(class Stage* s) { ownerStage = s; }

    
protected:
    std::unique_ptr<class MeshComponent> meshComp;
    std::unique_ptr<class ColliderComponent> collComp;
    bool isDisp;

    
    // オーナーのステージ
    class Stage* ownerStage;
};






#endif // __UFOACTOR_H
