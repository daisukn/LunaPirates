#include "MoaiEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

const int MAX_MAIBULLET = 12;

MoaiEnemy::MoaiEnemy(Application* a)
    : StageObjectActor(a)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/moai.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);


    // 爆発
    explosion = std::make_unique<ExplosionActor>(a);
    explosion->SetScale(2);
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/moai.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 弾幕
    for(int i = 0; i < MAX_MAIBULLET; i++)
    {
        bullet.emplace_back(std::make_unique<BulletActor>(a));
    }
    
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&MoaiEnemy::Behavior_0);
    BehaviorTable.push_back(&MoaiEnemy::Behavior_1);
    BehaviorTable.push_back(&MoaiEnemy::Behavior_2);
    BehaviorTable.push_back(&MoaiEnemy::Behavior_3);

    
    
}

MoaiEnemy::~MoaiEnemy()
{
    bullet.clear();
}

void MoaiEnemy::UpdateActor(float deltaTime)
{
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
}


void MoaiEnemy::Behavior_0(float deltaTime)
{
    if(!isDisp) return;
 
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
    
    
    cntLifetime++;
    
    if(cntLifetime == 300)
    {
        for(int j = 0;  j < 6; j++)
        {
            for(int i = 0; i < MAX_MAIBULLET; i++)
            {
                if(!bullet[i]->GetDisp())
                {
                    bullet[i]->SetAngle((float)j*60);
                    bullet[i]->Appear(GetPosition(), 0);
                    break;
                }
            }
        }
    }
        
}


void MoaiEnemy::Behavior_1(float deltaTime)
{
    
}

void MoaiEnemy::Behavior_2(float deltaTime)
{
    
}

void MoaiEnemy::Behavior_3(float deltaTime)
{
    
}
