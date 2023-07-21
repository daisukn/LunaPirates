#ifndef __MOAIENEMY_H
#define __MOAIENEMY_H

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include <memory>

class MoaiEnemy : public StageObjectActor
{
public:
    MoaiEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class ExplosionActor> explosion;
};



#endif // __MOAIENEMY_H

