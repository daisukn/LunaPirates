#pragma once

#include "StageObjectActor.h"
#include "BillboardComponent.h"
#include <vector>

class BulletActor : public StageObjectActor
{
public:
    BulletActor(class Application* a);
    void UpdateActor(float deltaTime) override;
    void SetAngle(float f) { angle = f; }
    float GetAngle() const { return angle; }

private:
    std::unique_ptr<class BillboardComponent> flare;
    float angle;
    float xSpeed;
    float ySpeed;
    float zSpeed;
    
    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (BulletActor::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
};
