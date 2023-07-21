#include "ShipEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "BoundingVolumeComponent.h"
#include "ColliderComponent.h"
#include "Mesh.h"
#include "Stage.h"


ShipEnemy::ShipEnemy(Application* a)
    : StageObjectActor(a)
{
/*
    skComp = std::make_unique<SkeletalMeshComponent>(this);
    skComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/ship.fbx"));
    skComp->SetAnimID(0, PLAY_CYCLIC);
    skComp->SetVisible(false);
    skComp->SetToonRender(true, 1.04f);
*/
    meshComp = std::make_unique<SkeletalMeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/ship2.fbx"));
    meshComp->SetAnimID(0, PLAY_CYCLIC);
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);
    
    
    // 爆発
    explosion = std::make_unique<ParticleComponent>(this);
    explosion->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/explosion.png"));


    SetScale(0.1f);
    
    
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/ship.fbx")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 300, 500), Vector3(1, 0.5, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
}

void ShipEnemy::UpdateActor(float deltaTime)
{
    if(isDisp)
    {
        if(state == StateNormal)
        {
            meshComp->SetVisible(true);
            collComp->GetBoundingVolume()->SetVisible(true);
            auto v = GetPosition();
            SetPosition(Vector3(v.x, v.y, v.z - 1.5));
            if(v.z < 0)
            {
                isDisp = false;
                meshComp->SetVisible(false);
                collComp->GetBoundingVolume()->SetVisible(false);
            }
        }
        else if(state == StateExploted)
        {
            if(!explosion->GetVisible())
            {
                isDisp = false;
            }
        }
        
        

    }
    collComp->SetDisp(isDisp);
    if(collComp->GetCollided())
    {
        if(collComp->GetTargetColliders()[0]->GetColliderType() == C_PLAYER
           || collComp->GetTargetColliders()[0]->GetColliderType() == C_LASER)
        {
            meshComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);
            collComp->SetCollided(false);
            
            state = StateExploted;
            explosion->CreateParticles(Vector3(0,0,0), 10, 0.8f, 0.5f, 20.0f, false);

        }

    }
 
}

