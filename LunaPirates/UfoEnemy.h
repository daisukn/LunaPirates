#pragma once

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include <memory>

class UfoEnemy : public StageObjectActor
{
public:
    UfoEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    float angY;
    float angle;
    std::unique_ptr<class ExplosionActor> explosion;

    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (UfoEnemy::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
    
};

