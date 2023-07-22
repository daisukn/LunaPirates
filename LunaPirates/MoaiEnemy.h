#ifndef __MOAIENEMY_H
#define __MOAIENEMY_H

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include "BulletActor.h"
#include <memory>
#include <vector>

class MoaiEnemy : public StageObjectActor
{
public:
    MoaiEnemy(class Application* a);
    ~MoaiEnemy();
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class ExplosionActor> explosion;
    std::vector<std::unique_ptr<class BulletActor>> bullet;
};



#endif // __MOAIENEMY_H

