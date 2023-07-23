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
    
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&BulletActor::Behavior_0);
    BehaviorTable.push_back(&BulletActor::Behavior_1);
    BehaviorTable.push_back(&BulletActor::Behavior_2);
    BehaviorTable.push_back(&BulletActor::Behavior_3);
}

void BulletActor::UpdateActor(float deltaTime)
{
    if(isDisp)
    {
        if (behaveType >= 0 && behaveType < BehaviorTable.size())
        {
            (this->*BehaviorTable[behaveType])(deltaTime);
        }
    }
}

void BulletActor::Behavior_0(float deltaTime)
{
    collComp->SetDisp(isDisp);
    
    float speed = -3.5f;
    float a = Math::ToRadians(angle);
    

        meshComp->SetVisible(true);
        flare->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x + cos(a) * speed/4, v.y + sin(a)*speed/4, v.z + speed));
        if(v.z < 0)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            flare->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);

        }
    
}

void BulletActor::Behavior_1(float deltaTime)
{
}
void BulletActor::Behavior_2(float deltaTime)
{
}
void BulletActor::Behavior_3(float deltaTime)
{
}
