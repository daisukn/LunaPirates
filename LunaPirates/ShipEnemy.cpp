#include "ShipEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "BoundingVolumeComponent.h"
#include "ColliderComponent.h"
#include "Mesh.h"

ShipEnemy::ShipEnemy(Application* a)
    : ObjectActor(a)
{
    skComp = std::make_unique<SkeletalMeshComponent>(this);
    skComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/ship.fbx"));
    skComp->SetAnimID(0, PLAY_CYCLIC);
    skComp->SetVisible(false);
    skComp->SetToonRender(true, 1.04f);
    SetScale(0.1f);
    
    
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_PLAYER);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/ship.fbx")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 300, 500), Vector3(1, 0.5, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
}

void ShipEnemy::UpdateActor(float deltaTime)
{
    if(isDisp)
    {
        skComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x, v.y, v.z - 1.5));
        if(v.z < 0)
        {
            isDisp = false;
            skComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);
        }
    }
}

