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
};



#endif // __SHIPENEMY_H


