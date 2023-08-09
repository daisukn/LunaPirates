#include "MoaiEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

const int MAX_MOAIBULLET = 50;
const int MAX_DONUTS = 8;

MoaiEnemy::MoaiEnemy(Application* a, Stage* s)
    : StageObjectActor(a, s)
    , forwardSpeed(0.0f)
    , anglerSpeed(0.0f)
    , upSpeed(0.0f)
    , angle(0.0f)
    , life(10)
    , cntShot(0)
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
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 弾幕
    for (int i = 0; i < MAX_MOAIBULLET; i++)
    {
        bullet.emplace_back(std::make_unique<BulletActor>(a, ownerStage));
    }
    
    for (int i = 0; i < MAX_DONUTS; i++)
    {
        donuts.emplace_back(std::make_unique<DonutsActor>(a, ownerStage));
    }
    
    // 移動用コンポーネント
    moveComp = std::make_unique<MoveComponent>(this);
    
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


void MoaiEnemy::Behavior_0(float deltaTime)
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
            ShotDonuts();
        }
    }


    moveComp->SetForwardSpeed(forwardSpeed);
    moveComp->SetAngularSpeed(anglerSpeed);
        
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

void MoaiEnemy::CheckCllider()
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


void MoaiEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    state = StateNormal;
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
    life = 10;
    cntShot = 0;
}

void MoaiEnemy::Disappear()
{
    StageObjectActor::Disappear();
    isDisp = false;
    meshComp->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
    collComp->SetCollided(false);
}

void MoaiEnemy::ShotDonuts()
{
    if (state != StateNormal)
    {
        return;
    }
    
    cntShot++;
    float xOffset = 10.f;
    if (cntShot % 2 == 0)
    {
        xOffset = -10.f;
    }
    
    Vector3 v = GetPosition();
    for (int i = 0; i < MAX_DONUTS; i++)
    {
        if (!donuts[i]->GetDisp())
        {
            donuts[i]->Appear(Vector3(v.x + xOffset, v.y, v.z-5 ), 0);
            break;
        }
    }
}
