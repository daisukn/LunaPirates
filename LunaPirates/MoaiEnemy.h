#ifndef __MOAIENEMY_H
#define __MOAIENEMY_H

#include "StageObjectActor.h"
#include "ParticleComponent.h"
#include <memory>

class MoaiEnemy : public StageObjectActor
{
public:
    MoaiEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class ParticleComponent> explosion;
};



#endif // __MOAIENEMY_H

