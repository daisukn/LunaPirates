#include "UfoEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "ColliderComponent.h"
#include "ParticleComponent.h"

UfoEnemy::UfoEnemy(Application* a)
    : StageObjectActor(a)
    , angY(0.0f)
    , angle(0.0f)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/ufo.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);
    
    
    // 爆発
    explosion = std::make_unique<ExplosionActor>(a);
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/ufo.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&UfoEnemy::Behavior_0);
    BehaviorTable.push_back(&UfoEnemy::Behavior_1);
    BehaviorTable.push_back(&UfoEnemy::Behavior_2);
    BehaviorTable.push_back(&UfoEnemy::Behavior_3);
}

void UfoEnemy::UpdateActor(float deltaTime)
{
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
}



void UfoEnemy::Behavior_0(float deltaTime)
{
    if(!isDisp) return;

    angY += 180 * deltaTime;
    angle += 1.f;
    
    Quaternion rot = Quaternion(Vector3(0,1,0), Math::ToRadians(angY));
    SetRotation(rot);
    
    if(state == StateNormal)
    {
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        
        auto v = GetPosition();
        SetPosition(Vector3(v.x+sin(angle/50)*6, v.y+sin(angle/20)*3, v.z - 1.5));
        if(v.z < 0)
        {
            isDisp = false;
            meshComp->SetVisible(false);
            collComp->GetBoundingVolume()->SetVisible(false);
        }
        
        
        collComp->SetDisp(isDisp);
        if(collComp->GetCollided())
        {
            for(auto col : collComp->GetTargetColliders())
            {
                if(col->GetColliderType() == C_PLAYER
                   || col->GetColliderType() == C_LASER)
                {
                    meshComp->SetVisible(false);
                    collComp->GetBoundingVolume()->SetVisible(false);
                    collComp->SetCollided(false);
                    
                    state = StateExploted;
                    explosion->Appear(GetPosition());
                    break;
                    
                }
            }

        }

        
    }
    else if(state == StateExploted)
    {
        if(!explosion->GetDisp())
        {
            isDisp = false;
        }
    }
        
 
}

void UfoEnemy::Behavior_1(float deltaTime)
{
    
}

void UfoEnemy::Behavior_2(float deltaTime)
{
    
}

void UfoEnemy::Behavior_3(float deltaTime)
{
    
}
