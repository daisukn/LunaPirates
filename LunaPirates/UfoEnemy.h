#ifndef __UFOENEMY_H
#define __UFOENEMY_H

#include "StageObjectActor.h"
#include "ParticleComponent.h"
#include <memory>

class UfoEnemy : public StageObjectActor
{
public:
    UfoEnemy(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    float ang;
    
    std::unique_ptr<class ParticleComponent> explosion;
    
};



#endif // __UFOENEMY_H

