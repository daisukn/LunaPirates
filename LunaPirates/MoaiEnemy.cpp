#include "MoaiEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"

const int MAX_MOAIBULLET = 50;
const int MAX_DONUTS = 5;

MoaiEnemy::MoaiEnemy(Application* a)
    : StageObjectActor(a)
    , forwardSpeed(0.0f)
    , anglerSpeed(0.0f)
    , upSpeed(0.0f)
    , angle(0.0f)
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
        bullet.emplace_back(std::make_unique<BulletActor>(a));
    }
    
    for (int i = 0; i < MAX_DONUTS; i++)
    {
        donuts.emplace_back(std::make_unique<DonutsActor>(a));
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
        
        
    if(state == StateNormal)
    {
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
        
        
        
        if(cntLifetime > 1000)
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
        }
    }
}


void MoaiEnemy::Behavior_0(float deltaTime)
{

    float speed = 300.f;
    
    if (cntLifetime < 90)
    {
        forwardSpeed = -speed;
    }
    else forwardSpeed = 0.f;

    if (cntLifetime == 120)
    {
        for (int i = 0; i < MAX_DONUTS; i++)
        {
            if (!donuts[i]->GetDisp())
            {
                donuts[i]->Appear(GetPosition(), 0);
                break;
            }
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


void MoaiEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
}

void MoaiEnemy::Disappear()
{
    
}
