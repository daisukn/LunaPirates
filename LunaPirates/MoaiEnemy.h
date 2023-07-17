#ifndef __MOAIENEMY_H
#define __MOAIENEMY_H

#include "ObjectActor.h"

class MoaiEnemy : public ObjectActor
{
public:
    MoaiEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
};



#endif // __MOAIENEMY_H

