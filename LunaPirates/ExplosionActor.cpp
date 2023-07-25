#include "ExplosionActor.h"
#include "Application.h"
#include "Renderer.h"


ExplosionActor::ExplosionActor(Application* a)
    : Actor(a)
    , isDisp(false)
    , cntLifetime(0)
{
    
    explosionPart = std::make_unique<ParticleComponent>(this);
    explosionPart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/explosion.png"));

    
    smokePart = std::make_unique<ParticleComponent>(this, 90);
    smokePart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/smoke.png"));

    
    flashPart = std::make_unique<ParticleComponent>(this);
    flashPart->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/flash.png"));
    flashPart->SetParticleSpeed(0.0f);

}

void ExplosionActor::UpdateActor(float deltaTime)
{
    if(!isDisp) { return; }
    
    cntLifetime++;
    
    if (cntLifetime == 1)
    {
        flashPart->CreateParticles(Vector3(0,0,0), 5, 0.4f, 0.2f, 20.0f);
    }
    if (cntLifetime == 5)
    {
        explosionPart->CreateParticles(Vector3(0,0,0), 10, 0.6f, 0.5f, 15.0f);
    }
    if (cntLifetime == 15)
    {
        smokePart->CreateParticles(Vector3(0,0.1,0), 5, 0.6f, 0.4f, 20.0f);
    }
    if (cntLifetime == 60)
    {
        isDisp = false;
    }
    
    auto v = GetPosition();
    SetPosition(Vector3(v.x, v.y - 0.5f, v.z));
 }

void ExplosionActor::Appear(Vector3 pos)
{
    cntLifetime = 0;
    isDisp = true;
    SetPosition(pos);
 }
