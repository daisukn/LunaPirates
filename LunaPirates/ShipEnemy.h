#ifndef __SHIPENEMY_H
#define __SHIPENEMY_H

#include "StageObjectActor.h"
#include "ParticleComponent.h"
#include <memory>


class ShipEnemy : public StageObjectActor
{
public:
    ShipEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class ParticleComponent> explosion;
};



#endif // __SHIPENEMY_H


