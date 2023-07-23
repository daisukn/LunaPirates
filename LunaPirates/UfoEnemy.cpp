#include "UfoEnemy.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "ColliderComponent.h"
#include "ParticleComponent.h"

const int MAX_BULLET = 30;

UfoEnemy::UfoEnemy(Application* a)
    : StageObjectActor(a)
    , rotY(0.0f)
    , angle(0.0f)
    , xSpeed(0.0f)
    , ySpeed(0.0f)
    , zSpeed(0.0f)
    , shotAngle(0.0f)
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
        bullets.push_back(std::make_unique<BulletActor>(a));
    }
    
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_ENEMY);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/ufo.lwo")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 1, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    
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
    angle = 0.0f;
    rotY = 0.0f;
}

void UfoEnemy::UpdateActor(float deltaTime)
{
    if (!isDisp) { return; }
    
    cntLifetime++;
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
        
        
    if(state == StateNormal)
    {
        meshComp->SetVisible(true);
        collComp->GetBoundingVolume()->SetVisible(true);
            
        auto v = GetPosition();
        SetPosition(Vector3(v.x+sin(Math::ToRadians(angle))*xSpeed, v.y+sin(Math::ToRadians(angle/2))*ySpeed, v.z - zSpeed));

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



void UfoEnemy::Behavior_0(float deltaTime)
{
    rotY += 60.f * deltaTime;
    angle += 240.f * deltaTime;
    
    Quaternion rot = Quaternion(Vector3(0,1,0), Math::ToRadians(rotY));
    SetRotation(rot);
    
    zSpeed = 0.0f;
    xSpeed = 0.0f;
    ySpeed = 150.f * deltaTime;
    
    if(cntLifetime < 120)
    {
        zSpeed = 300 * deltaTime;
        ySpeed = 0.0f;
    }
    else if(cntLifetime < 700)
    {
        zSpeed = 0.0f;
        xSpeed = 0.0f;
        ySpeed = 0.0f;//100.f * deltaTime;
        
        int prevCnt = 120;
        if( (cntLifetime - prevCnt) % 3 == 0)
        {
            //ShotCircle();
            ShotLiner();
        }
    }
    else
    {
        zSpeed = 100 * deltaTime;
        xSpeed = 150 * deltaTime;
        ySpeed = 0.0f;
    }
    

}

void UfoEnemy::Behavior_1(float deltaTime)
{
    rotY += 60.f * deltaTime;
    angle += 240.f * deltaTime;
    
    Quaternion rot = Quaternion(Vector3(0,1,0), Math::ToRadians(rotY));
    SetRotation(rot);
    
    zSpeed = 0.0f;
    xSpeed = 0.0f;
    ySpeed = 150.f * deltaTime;
    
    if(cntLifetime < 120)
    {
        zSpeed = 300 * deltaTime;
        ySpeed = 0.0f;
    }
    else if(cntLifetime < 700)
    {
        zSpeed = 0.0f;
        xSpeed = 0.0f;
        ySpeed = 100.f * deltaTime;
        
        int prevCnt = 120;
        if( (cntLifetime - prevCnt) % 50 == 0)
        {
            //ShotLiner();
            ShotCircle();
        }
    }
    else
    {
        zSpeed = 100 * deltaTime;
        xSpeed = 150 * deltaTime;
        ySpeed = 0.0f;
    }
}

void UfoEnemy::Behavior_2(float deltaTime)
{
    
}

void UfoEnemy::Behavior_3(float deltaTime)
{
    
}

void UfoEnemy::ShotLiner()
{
    shotAngle += 30;
    for(int i = 0; i < MAX_BULLET; i++)
    {
        if(!bullets[i]->GetDisp())
        {
            auto v = GetPosition();
            auto a = Math::ToRadians(shotAngle);
            bullets[i]->Appear(Vector3(v.x + cos(a)*10, v.y + sin(a)*10, v.z), 0);
            break;
        }
    }

}


void UfoEnemy::ShotCircle()
{
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
}

