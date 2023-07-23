#ifndef __SHIPENEMY_H
#define __SHIPENEMY_H

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include <memory>


class ShipEnemy : public StageObjectActor
{
public:
    ShipEnemy(class Application* a);
    virtual ~ShipEnemy();
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class ExplosionActor> explosion;
    
    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (ShipEnemy::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
};



#endif // __SHIPENEMY_H


