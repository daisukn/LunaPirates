#pragma once

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include "BulletActor.h"
#include <memory>
#include <vector>

class UfoEnemy : public StageObjectActor
{
public:
    UfoEnemy(class Application* a);
    ~UfoEnemy();
    void UpdateActor(float deltaTime) override;
    void Appear(Vector3 pos, int type) override;
private:
    float rotY;
    float angle;
    float xSpeed;
    float ySpeed;
    float zSpeed;
    float shotAngle;
    
    std::unique_ptr<class ExplosionActor> explosion;
    std::vector<std::unique_ptr<class BulletActor>> bullets;

    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (UfoEnemy::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
    
    void ShotCircle();
    void ShotLiner();
};

