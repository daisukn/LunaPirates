#include "LaserActor.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

LaserActor::LaserActor(Application* a)
    : StageObjectActor(a)
{
    meshComp = std::make_unique<MeshComponent>(this, false, true);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/laser.lwo"));
    meshComp->SetVisible(false);
    //meshComp->SetToonRender(true, 1.04f);
    
    SetScale(0.05f);

    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_LASER);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/laser.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    
}

void LaserActor::UpdateActor(float deltaTime)
{
    collComp->SetDisp(isDisp);
    if(isDisp)
    {
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x, v.y, v.z+20));
        if(v.z > 1000)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);

        }
    }
}

