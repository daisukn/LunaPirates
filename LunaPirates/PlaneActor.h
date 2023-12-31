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
    PlaneActor(class Application* app, class Stage* s);

    void ActorInput(const struct InputState& state) override;
    void UpdateActor(float deltaTime) override;

    void SetMeshVisible(bool visible);
    
    int GetLife() const { return life; }
    int GetMaxLife() const { return maxLife; }
    
    
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
    void DamageEffect(bool b);
    
    int life;
    int maxLife;

};

