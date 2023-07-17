#ifndef __UFOENEMY_H
#define __UFOENEMY_H

#include "StageObjectActor.h"


class UfoEnemy : public StageObjectActor
{
public:
    UfoEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    float ang;
};



#endif // __UFOENEMY_H

