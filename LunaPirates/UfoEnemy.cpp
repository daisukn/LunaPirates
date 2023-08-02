#include "UfoEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "ColliderComponent.h"
#include "ParticleComponent.h"
#include "MoveComponent.h"

const int MAX_BULLET = 50;

UfoEnemy::UfoEnemy(Application* a)
    : StageObjectActor(a)
    , forwardSpeed(0.0f)
    , anglerSpeed(0.0f)
    , upSpeed(0.0f)
    , angle(0.0f)
{
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/ufo.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetToonRender(true, 1.04f);
    
    // 爆発
    explosion = std::make_unique<ExplosionActor>(a);
    
    // 弾幕
    for(int i = 0; i < MAX_BULLET; i++)
    {
        bullets.emplace_back(std::make_unique<BulletActor>(a));
    }
    
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/ufo.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
    
    // 移動用コンポーネント
    moveComp = std::make_unique<MoveComponent>(this);
    
    
    // 関数テーブル初期化
    BehaviorTable.push_back(&UfoEnemy::Behavior_0);
    BehaviorTable.push_back(&UfoEnemy::Behavior_1);
    BehaviorTable.push_back(&UfoEnemy::Behavior_2);
    BehaviorTable.push_back(&UfoEnemy::Behavior_3);
}

UfoEnemy::~UfoEnemy()
{
    bullets.clear();
}

void UfoEnemy::Appear(Vector3 pos, int type)
{
    StageObjectActor::Appear(pos, type);
    anglerSpeed = 0.0f;
    forwardSpeed = 0.0f;
    angle = 0.0f;
    moveComp->Reset();
}

void UfoEnemy::Disappear()
{
    StageObjectActor::Disappear();
    meshComp->SetVisible(false);
    collComp->GetBoundingVolume()->SetVisible(false);
}


void UfoEnemy::CheckCllider()
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

void UfoEnemy::UpdateActor(float deltaTime)
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



void UfoEnemy::Behavior_0(float deltaTime)
{


    float speed = 150.f;
    
    if (cntLifetime < 100)
    {
        forwardSpeed = speed;
//        std::cout << GetPosition().z << std::endl;

    }
    if (cntLifetime == 220)
    {
        forwardSpeed = speed;
        if (GetPosition().x > 0)
        {
            anglerSpeed = -75;
        }
        else
        {
            anglerSpeed = 75;
        }
    }
    if (cntLifetime == 440)
    {
        anglerSpeed = 0.0f;
    }
    
    if (cntLifetime > 220 && cntLifetime < 440)
    {
        if (cntLifetime % 10 == 0)
        {
            ShotLiner();
        }
    }
    moveComp->SetForwardSpeed(forwardSpeed);
    moveComp->SetAngularSpeed(anglerSpeed);


}

void UfoEnemy::Behavior_1(float deltaTime)
{

    float speed = 100.f;
    angle += 5.f;
    
    if (cntLifetime < 100)
    {
        forwardSpeed = -speed;
//        std::cout << GetPosition().z << std::endl;
        
        if (cntLifetime % 5 == 0)
        {
            ShotCircle();
        }
    }
    if (cntLifetime > 240)
    {
        forwardSpeed = 0;
        upSpeed = 100 * sin(Math::ToRadians(angle));
    }

    
    if (cntLifetime > 240)
    {
        if (cntLifetime % 5 == 0)
        {
            ShotLiner();
        }
    }
    if (cntLifetime > 500)
    {
        forwardSpeed = -speed;
    }
    moveComp->SetForwardSpeed(forwardSpeed);
    moveComp->SetUpSpeed(upSpeed);

/*
    auto v = GetPosition();
    v += Vector3::UnitY * upSpeed;
    SetPosition(v);
 */
}

void UfoEnemy::Behavior_2(float deltaTime)
{

}

void UfoEnemy::Behavior_3(float deltaTime)
{
    
}





void UfoEnemy::ShotLiner()
{
    if (state != StateNormal)
    {
        return;
    }
    for (int i = 0; i < MAX_BULLET; i++)
    {
        if(!bullets[i]->GetDisp())
        {
            auto v = GetPosition();
            bullets[i]->Appear(Vector3(v.x, v.y, v.z), 0);
            break;
        }
    }

}


void UfoEnemy::ShotCircle()
{
    if (state != StateNormal)
    {
        return;
    }

    
    for(int j = 0;  j < 6; j++)
    {
        for(int i = 0; i < MAX_BULLET; i++)
        {
            if(!bullets[i]->GetDisp())
            {
                bullets[i]->SetAngle((float)j*60);
                bullets[i]->Appear(GetPosition(), 1);
                break;
            }
        }
    }
    for(int i = 0; i < MAX_BULLET; i++)
    {
        if(!bullets[i]->GetDisp())
        {
            bullets[i]->Appear(GetPosition(), 0);
            break;
        }
    }
}

