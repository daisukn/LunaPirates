#pragma once

#include "StageObjectActor.h"
#include "BillboardComponent.h"
#include "MeshComponent.h"
#include "ExplosionActor.h"
#include <vector>
#include <memory>

class DonutsActor : public StageObjectActor
{
public:
    DonutsActor(class Application* a);
    void UpdateActor(float deltaTime) override;
    void SetAngle(float f) { angle = f; }
    float GetAngle() const { return angle; }
    
    virtual void Appear(Vector3 pos, int type) override;
    virtual void Disappear() override;

private:

    std::unique_ptr<class MeshComponent> lightning;
    std::unique_ptr<class ExplosionActor> explosion;
    
    float angle;
    float xSpeed;
    float ySpeed;
    float zSpeed;
    float scale;
    
    void CheckCllider();
    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (DonutsActor::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
};
