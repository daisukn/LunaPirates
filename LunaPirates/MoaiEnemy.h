#ifndef __MOAIENEMY_H
#define __MOAIENEMY_H

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include "BulletActor.h"
#include <memory>
#include <vector>
#include <functional>

class MoaiEnemy : public StageObjectActor
{
public:
    MoaiEnemy(class Application* a);
    ~MoaiEnemy();
    void UpdateActor(float deltaTime) override;
    


private:
    std::unique_ptr<class ExplosionActor> explosion;
    std::vector<std::unique_ptr<class BulletActor>> bullet;
    
    
    // 各行動パターン
    void Behavior_0(float deltaTime);
    void Behavior_1(float deltaTime);
    void Behavior_2(float deltaTime);
    void Behavior_3(float deltaTime);
    // 関数テーブル
    typedef void (MoaiEnemy::*Behavior)(float deltaTime);
    std::vector<Behavior> BehaviorTable;
};



#endif // __MOAIENEMY_H

