#include "BulletActor.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "BillboardComponent.h"

BulletActor::BulletActor(Application* a)
    : StageObjectActor(a)
    , angle(0.0f)
{
    meshComp = std::make_unique<MeshComponent>(this, false, true);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/bullet.lwo"));
    meshComp->SetVisible(false);
    //meshComp->SetToonRender(true, 1.04f);
    
    SetScale(0.15f);

    flare = std::make_unique<BillboardComponent>(this);
    flare->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/flare.png"));
    flare->SetScale(0.15f);
    
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_BULLET);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/bullet.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
}



void BulletActor::UpdateActor(float deltaTime)
{
    collComp->SetDisp(isDisp);
    flare->SetVisible(true);
    
    float speed = -5.0f;
    float a = Math::ToRadians(angle);
    
    if(isDisp)
    {
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x + cos(a) * speed/3, v.y + sin(a)*speed/3, v.z + speed));
        if(v.z < 0)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);

        }
    }
}

