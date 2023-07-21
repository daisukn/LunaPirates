#include "MoaiEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

MoaiEnemy::MoaiEnemy(Application* a)
    : StageObjectActor(a)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/moai.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);

    
    // 爆発
    explosion = std::make_unique<ParticleComponent>(this);
    explosion->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/explosion.png"));
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/moai.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    
}

void MoaiEnemy::UpdateActor(float deltaTime)
{
    if(isDisp)
    {
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x, v.y, v.z-2));
        if(v.z < 0)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);

        }
    }
    collComp->SetDisp(isDisp);
    if(collComp->GetCollided())
    {
        if(collComp->GetTargetColliders()[0]->GetColliderType() == C_PLAYER
           || collComp->GetTargetColliders()[0]->GetColliderType() == C_LASER)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);
            collComp->SetCollided(false);
            
            explosion->CreateParticles(Vector3(0,0,0), 10, 0.8f, 0.5f, 20.0f, false);

        }

    }
 
}
