#ifndef __UFOENEMY_H
#define __UFOENEMY_H

#include "StageObjectActor.h"
#include "ExplosionActor.h"
#include <memory>

class UfoEnemy : public StageObjectActor
{
public:
    UfoEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    float ang;
    std::unique_ptr<class ExplosionActor> explosion;

    
};



#endif // __UFOENEMY_H

