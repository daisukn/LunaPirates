#pragma once

#include "StageObjectActor.h"
#include "BillboardComponent.h"
#include <vector>

class DonutsActor : public StageObjectActor
{
public:
    DonutsActor(class Application* a);
    void UpdateActor(float deltaTime) override;
    void SetAngle(float f) { angle = f; }
    float GetAngle() const { return angle; }

private:

    float angle;
    float xSpeed;
    float ySpeed;
    float zSpeed;
    float scale;
    
    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (DonutsActor::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
};
