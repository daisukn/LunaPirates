#ifndef __LASERACTOR_H
#define __LASERACTOR_H

#include "StageObjectActor.h"

class LaserActor : public StageObjectActor
{
public:
    LaserActor(class Application* a);
    void UpdateActor(float deltaTime) override;
};


#endif // __LASERACTOR_H
