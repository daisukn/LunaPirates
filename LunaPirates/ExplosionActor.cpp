#include "ExplosionActor.h"
#include "Application.h"
#include "Renderer.h"


ExplosionActor::ExplosionActor(Application* a)
    : Actor(a)
    , isDisp(false)
{
    explosionPart = std::make_unique<ParticleComponent>(this);
    explosionPart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/explosion.png"));
    
    smokePart = std::make_unique<ParticleComponent>(this, 90);
    smokePart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/smoke.png"));

}

void ExplosionActor::UpdateActor(float deltaTime)
{
    if(isDisp)
    {
        if(!explosionPart->GetVisible())
        {
            isDisp = false;
        }
        
    }
}

void ExplosionActor::Appear(Vector3 pos)
{
    isDisp = true;
    SetPosition(pos);
    explosionPart->CreateParticles(Vector3(0,0,0), 10, 0.8f, 0.5f, 15.0f);
    smokePart->CreateParticles(Vector3(0,0.1,0), 5, 0.8f, 0.4f, 20.0f, ParticleComponent::P_SMOKE);
}
