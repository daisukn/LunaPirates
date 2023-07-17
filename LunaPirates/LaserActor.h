#ifndef __LASERACTOR_H
#define __LASERACTOR_H

#include "ObjectActor.h"

class LaserActor : public ObjectActor
{
public:
    LaserActor(class Application* a);
    void UpdateActor(float deltaTime) override;
};


#endif // __LASERACTOR_H
