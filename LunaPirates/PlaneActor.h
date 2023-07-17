#ifndef __PLANEACTOR_H
#define __PLANEACTOR_H

#include "Actor.h"
#include "SkeletalMeshComponent.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "ColliderComponent.h"
#include <memory>


// 飛行機
class PlaneActor : public Actor
{
public:
    PlaneActor(class Application* app);

    void ActorInput(const struct InputState& state) override;
    void UpdateActor(float deltaTime) override;

    void SetMeshVisible(bool visible);
    
    void SetOwnerStage(class Stage* s) { ownerStage = s; }
    
private:
    //  前フレームの場所
    Vector3 prevPos;
    
    // オーナーのステージ
    class Stage* ownerStage;
    
    // モーションID
    int animID;
    
    std::unique_ptr<class SkeletalMeshComponent> meshComp;
    std::unique_ptr<class MoveComponent> moveComp;
    std::unique_ptr<class FollowCamera> cameraComp;
    
    
    std::unique_ptr<class ColliderComponent> collComp;
    
    // フィールド移動用
    void FieldMove(const struct InputState& state);
    // モーションを適用
    void ApplyMotion();
    
    // 動作可能=true 動作不可能=false
    bool isMovable;

};





#endif // __PLANEACTOR_H
