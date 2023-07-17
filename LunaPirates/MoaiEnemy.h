#ifndef __MOAIENEMY_H
#define __MOAIENEMY_H

#include "StageObjectActor.h"

class MoaiEnemy : public StageObjectActor
{
public:
    MoaiEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
};



#endif // __MOAIENEMY_H

