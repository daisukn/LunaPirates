#include "LaserActor.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

LaserActor::LaserActor(Application* a)
    : StageObjectActor(a)
{
    meshComp = std::make_unique<MeshComponent>(this, false, true);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/laser.lwo"));
    meshComp->SetVisible(false);
    //meshComp->SetToonRender(true, 1.04f);
    
    SetScale(0.05f);
    
    
    efectMesh = std::make_unique<MeshComponent>(this, false, true);
    efectMesh->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/stripe.lwo"));
    efectMesh->SetVisible(false);
    efectMesh->SetScale(1.05f);
    efectMesh->SetBlendAdd(true);
    
    
    particle = std::make_unique<ParticleComponent>(this);
    particle->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/laser_part.png"));

    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_LASER);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/laser.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    
}

void LaserActor::UpdateActor(float deltaTime)
{
    collComp->SetDisp(isDisp);
    if(isDisp)
    {
        meshComp->SetVisible(true);
        efectMesh->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x, v.y, v.z+20));
        if(v.z > 1000)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            efectMesh->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);

        }
    }
}

void LaserActor::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    particle->CreateParticles(Vector3(0,0,-100), 10, 5.f, 0.2f, 60.0f);
    particle->SetParticleSpeed(5);
}


