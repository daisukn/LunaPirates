#pragma once

#include "Actor.h"
#include "Math.h"
#include "ParticleComponent.h"
#include <memory>


class ExplosionActor : public Actor
{
public:
    ExplosionActor(class Application* a);
    
    void UpdateActor(float deltaTime) override;
    
    void Appear(Vector3 pos);
    
    bool GetDisp() const { return isDisp; }
    void SetDisp(bool b) { isDisp = b; }
    
private:
    
    std::unique_ptr<class ParticleComponent> flashPart;
    std::unique_ptr<class ParticleComponent> explosionPart;
    std::unique_ptr<class ParticleComponent> smokePart;

    
    bool isDisp;
    int cntLifetime;
    
};
