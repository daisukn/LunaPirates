#include "DonutsActor.h"
#include "Application.h"
#include "Renderer.h"
#include "ColliderComponent.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

DonutsActor::DonutsActor(Application* a, Stage* s)
    : StageObjectActor(a, s)
    , angle(0.0f)
    , xSpeed(0.0f)
    , ySpeed(0.0f)
    , zSpeed(0.0f)
    , scale(0.15f)
{
    
    
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/donuts.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true);
    SetScale(scale);
    
    lightning = std::make_unique<MeshComponent>(this, false, MESH_EFFECT);
    lightning->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/donuts2.lwo"));
    lightning->SetBlendAdd(true);
    lightning->SetVisible(false);

    // 爆発
    explosion = std::make_unique<ExplosionActor>(a);
    
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
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
    cntLifetime++;
        
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
    
    
    if(state == StateNormal)
    {
        
        auto v = GetPosition();
        SetPosition(Vector3(v.x + xSpeed, v.y + ySpeed, v.z + zSpeed));

        
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        
        
        
        if (GetPosition().z < 0)
        {
            Disappear();
        }
        CheckCllider();
        
        
    }
    else if(state == StateExploted)
    {
        
        if(!explosion->GetDisp())
        {
            isDisp = false;
            Disappear();
        }
    }
    
}



void DonutsActor::Behavior_0(float deltaTime)
{
    if(state != StateNormal) return;
    
    
    if (cntLifetime < 30)
    {
        zSpeed = 0.f;
        scale *= 1.01f;
        SetScale(scale);
    }
    else
    {
        lightning->SetVisible(true);
    }
    
    if (cntLifetime > 50)
    {
        angle += 10.f;
        Quaternion rot(Vector3::UnitX, Math::ToRadians(angle));
        SetRotation(rot);
        
        zSpeed = -200*deltaTime;

    }

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

void DonutsActor::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    scale = 0.15f;
    angle = 0.0f;
    
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    cntLifetime = 0;
    state = StateNormal;
}

void DonutsActor::Disappear()
{
    isDisp = false;
    meshComp->SetVisible(false);
    lightning->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetDisp(false);
}

void DonutsActor::CheckCllider()
{
    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_LASER || col->GetColliderType() == C_PLAYER)
            {
                meshComp->SetVisible(false);
                lightning->SetVisible(false);
                collComp->GetBoundingVolume()->SetVisible(false);
                collComp->SetCollided(false);
                state = StateExploted;
                explosion->Appear(GetPosition());
                break;
                
            }
        }

    }
}
