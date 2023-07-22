#ifndef __BULLETACTOR_H
#define __BULLETACTOR_H

#include "StageObjectActor.h"

class BulletActor : public StageObjectActor
{
public:
    BulletActor(class Application* a);
    void UpdateActor(float deltaTime) override;
};


#endif // __BULLETACTOR_H
