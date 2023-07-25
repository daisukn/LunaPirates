#include "BulletActor.h"
#include "Application.h"
#include "Renderer.h"
#include "BoundingVolumeComponent.h"
#include "Mesh.h"
#include "BillboardComponent.h"

BulletActor::BulletActor(Application* a)
    : StageObjectActor(a)
    , angle(0.0f)
    , xSpeed(0.0f)
    , ySpeed(0.0f)
    , zSpeed(0.0f)
{
    
    
    meshComp = std::make_unique<MeshComponent>(this);
    meshComp->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/bullet.lwo"));
    meshComp->SetVisible(false);
    meshComp->SetBlendAdd(true);
    //meshComp->SetToonRender(true, 1.04f);

    
    blackMesh = std::make_unique<MeshComponent>(this);
    blackMesh->SetMesh(GetApp()->GetRenderer()->GetMesh("Assets/Models/black_ball.lwo"));
    blackMesh->SetVisible(false);
    blackMesh->SetScale(0.8f);


    
    SetScale(0.15f);

    flare = std::make_unique<BillboardComponent>(this);
    flare->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/flare.png"));
    flare->SetScale(0.1f);
    
    
    // コライダー
    collComp = std::make_unique<ColliderComponent>(this);
    collComp->SetColliderType(C_BULLET);
    collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/bullet.lwo")->GetVertexArray());
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
    if (!isDisp) { return; }
        
    if (behaveType >= 0 && behaveType < BehaviorTable.size())
    {
        (this->*BehaviorTable[behaveType])(deltaTime);
    }
    
    
    collComp->SetDisp(true);
    meshComp->SetVisible(true);
    //blackMesh->SetVisible(true);
    flare->SetVisible(true);
    collComp->GetBoundingVolume()->SetVisible(true);
    auto v = GetPosition();
    SetPosition(Vector3(v.x + xSpeed, v.y + ySpeed, v.z + zSpeed));

    if(v.z < 0)
    {
        isDisp = false;
        meshComp->SetVisible(false);
        //blackMesh->SetVisible(false);
        flare->SetVisible(false);
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
                //blackMesh->SetVisible(false);
                flare->SetVisible(false);
                collComp->GetBoundingVolume()->SetVisible(false);
                collComp->SetDisp(false);
                break;
                
            }
        }

    }
    
}



void BulletActor::Behavior_0(float deltaTime)
{
    float speed = 10.0f;

    zSpeed = -speed;
}

void BulletActor::Behavior_1(float deltaTime)
{
    float speed = 30.0f;
    
    xSpeed = cos(Math::ToRadians(angle)) * speed * deltaTime;
    ySpeed = sin(Math::ToRadians(angle)) * speed * deltaTime;
    zSpeed = -6.f;

}

void BulletActor::Behavior_2(float deltaTime)
{
}
void BulletActor::Behavior_3(float deltaTime)
{
}

