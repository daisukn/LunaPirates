#include "DonutsActor.h"
#include "Application.h"
#include "Renderer.h"
#include "ColliderComponent.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

DonutsActor::DonutsActor(Application* a)
    : StageObjectActor(a)
    , angle(0.0f)
    , xSpeed(0.0f)
    , ySpeed(0.0f)
    , zSpeed(0.0f)
    , scale(0.15f)
{
    
    
    meshComp = std::make_unique<MeshComponent>(this, false, MESH_EFFECT);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/donuts.lwo"));
    meshComp->SetVisible(false);
    SetScale(scale);


    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_BULLET);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/donuts.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&DonutsActor::Behavior_0);
    BehaviorTable.push_back(&DonutsActor::Behavior_1);
    BehaviorTable.push_back(&DonutsActor::Behavior_2);
    BehaviorTable.push_back(&DonutsActor::Behavior_3);
}

void DonutsActor::UpdateActor(float deltaTime)
{
    if (!isDisp) { return; }
        
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
    
    
    collComp->SetDisp(true);
    meshComp->SetVisible(true);

    collComp->GetBoundingVolume()->SetVisible(true);
    auto v = GetPosition();
    SetPosition(Vector3(v.x + xSpeed, v.y + ySpeed, v.z + zSpeed));

    if(v.z < 0)
    {
        isDisp = false;
        meshComp->SetVisible(false);
        collComp->GetBoundingVolume()->SetVisible(false);
        collComp->SetDisp(false);
    }
    
    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_PLAYER)
            {
                isDisp = false;
                meshComp->SetVisible(false);
                collComp->GetBoundingVolume()->SetVisible(false);
                collComp->SetDisp(false);
                break;
                
            }
        }

    }
    
}



void DonutsActor::Behavior_0(float deltaTime)
{
    scale *= 1.01f;
    SetScale(scale);

}

void DonutsActor::Behavior_1(float deltaTime)
{


}

void DonutsActor::Behavior_2(float deltaTime)
{
}
void DonutsActor::Behavior_3(float deltaTime)
{
}

