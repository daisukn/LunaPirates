#pragma once

#include "StageObjectActor.h"
#include "SkeletalMeshComponent.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include "ColliderComponent.h"
#include "TargetScopeActor.h"
#include "LaserActor.h"
#include "SpriteComponent.h"
#include "Actor.h"

#include <memory>


// 飛行機
class PlaneActor : public StageObjectActor
{
public:
    PlaneActor(class Application* app);

    void ActorInput(const struct InputState& state) override;
    void UpdateActor(float deltaTime) override;

    void SetMeshVisible(bool visible);
    
    
private:
   
    // モーションID
    int animID;
    
    //std::unique_ptr<class SkeletalMeshComponent> skeltalMeshComp;
    std::unique_ptr<class MoveComponent> moveComp;
    std::unique_ptr<class FollowCamera> cameraComp;
    std::unique_ptr<class MeshComponent> lightning;
    
    std::unique_ptr<class TargetScopeActor> scopeActor;
    

    // フィールド移動用
    void FieldMove(const struct InputState& state);

    
    // 動作可能=true 動作不可能=false
    bool isMovable;
    
    void ShotLaser();
    std::vector<std::unique_ptr<class StageObjectActor>> laserActor;
    
    int barrierCnt;
    int flashCnt;
    void DamageEffect(bool b);

};

