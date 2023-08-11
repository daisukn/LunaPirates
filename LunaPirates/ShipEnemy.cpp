#include "ShipEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "BoundingVolumeComponent.h"
#include "ColliderComponent.h"
#include "Mesh.h"
#include "Stage.h"

const int MAX_SHIPBULLET = 50;

ShipEnemy::ShipEnemy(Application* a, Stage* s)
    : StageObjectActor(a, s)
{

    meshComp = std::make_unique<SkeletalMeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/ship2.fbx"));
    meshComp->SetAnimID(0, PLAY_CYCLIC);
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);
    SetScale(0.1f);
    
    // 爆発
    explosion = std::make_unique<ExplosionActor>(a);
    explosion->SetScale(4);


    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/ship.fbx")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 300, 500), Vector3(1, 0.5, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 弾幕
    for (int i = 0; i < MAX_SHIPBULLET; i++)
    {
        bullet.emplace_back(std::make_unique<BulletActor>(a, ownerStage));
    }
    
    // 移動用コンポーネント
    moveComp = std::make_unique<MoveComponent>(this);
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&ShipEnemy::Behavior_0);
    BehaviorTable.push_back(&ShipEnemy::Behavior_1);
    BehaviorTable.push_back(&ShipEnemy::Behavior_2);
    BehaviorTable.push_back(&ShipEnemy::Behavior_3);
    
}

ShipEnemy::~ShipEnemy()
{
    
}



void ShipEnemy::UpdateActor(float deltaTime)
{
    if (!isDisp) { return; }
    cntLifetime++;
    

    // 行動を反映
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
        
        
    if (state == StateNormal)
    {
        if (GetPosition().z < 0)
        {
            Disappear();
        }
        CheckCllider();
        
        
    }
    else if (state == StateExploted)
    {
        
        if (!explosion->GetDisp())
        {
            isDisp = false;
            Disappear();
        }
    }
}


void ShipEnemy::Behavior_0(float deltaTime)
{
    if (cntLifetime < 90)
    {
        forwardSpeed = -300.f;
    }
    else if (cntLifetime < 600)
    {
        forwardSpeed = 0.f;
    }
    else
    {
        forwardSpeed = -100.f;
    }

    if (cntLifetime > 120 && cntLifetime < 600)
    {
        if (cntLifetime % 30 == 0)
        {
            //ShotDonuts();
        }
    }


    moveComp->SetForwardSpeed(forwardSpeed);
    moveComp->SetAngularSpeed(anglerSpeed);
        
        
     
}

void ShipEnemy::Behavior_1(float deltaTime)
{
    
}

void ShipEnemy::Behavior_2(float deltaTime)
{
    
}

void ShipEnemy::Behavior_3(float deltaTime)
{
    
}


void ShipEnemy::CheckCllider()
{

    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_PLAYER
                   || col->GetColliderType() == C_LASER)
            {
                
                life--;
                if (life <= 0)
                {
                    meshComp->SetVisible(false);
                    collComp->GetBoundingVolume()->SetVisible(false);
                    collComp->SetCollided(false);
                    
                    state = StateExploted;
                    explosion->Appear(GetPosition());
                }
                break;
                    
            }
        }

    }
}

void ShipEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    state = StateNormal;
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
    life = 100;
}

void ShipEnemy::Disappear()
{
    StageObjectActor::Disappear();
    isDisp = false;
    meshComp->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetCollided(false);
}
