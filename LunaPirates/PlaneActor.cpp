
#include "PlaneActor.h"
#include "MeshComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "SkeletalMeshComponent.h"
#include "Mesh.h"
#include "InputSystem.h"
#include "FollowCamera.h"
#include "MoveComponent.h"
#include "BoundingVolumeComponent.h"
#include "CloudStage.h"
#include "TargetScopeActor.h"

const int MAX_LASER = 20;

const float AREA_LIMIT_H = 45.f;
const float AREA_LIMIT_W = 75.f;

PlaneActor::PlaneActor(Application* app)
    : StageObjectActor(app)
    , animID(0)
    , isMovable(true)
    , barrierCnt(0)
 {
     // メッシュ初期化
     meshComp = std::make_unique<SkeletalMeshComponent>(this);
     meshComp->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/plane.fbx"));
     meshComp->SetAnimID(animID, PLAY_CYCLIC);
     meshComp->SetToonRender(true);
     
  
     // 場所調整
     SetPosition(Vector3(0.0f, 0.0f, -0.0f));
     //SetScale(0.01);
    
     // カメラ初期化
     cameraComp = std::make_unique<FollowCamera>(this);
     // 移動コンポーネント
     moveComp = std::make_unique<MoveComponent>(this);

    
     // コライダー
     collComp = std::make_unique<ColliderComponent>(this);
     collComp->SetColliderType(C_PLAYER);
     collComp->GetBoundingVolume()->ComputeBoundingVolume(app->GetRenderer()->GetMesh("Assets/Models/plane.fbx")->GetVertexArray());
     collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 0.5, 1));
     collComp->GetBoundingVolume()->CreateVArray();
     collComp->GetBoundingVolume()->SetVisible(true);

    
     // レーザー
     for(int i = 0; i < MAX_LASER; i++)
     {
         laserActor.emplace_back( std::make_unique<LaserActor>(app));
     }
     

     // ターゲットスコープ
     scopeActor = std::make_unique<TargetScopeActor>(app);
     scopeActor->SetOwnerStage(ownerStage);
     scopeActor->SetDisp(true);
     

     // 稲妻
     lightning = std::make_unique<MeshComponent>(this, false, MESH_EFFECT);
     lightning->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/lightning.lwo"));
     lightning->SetScale(0.01f);
     //lightning->SetBlendAdd(true);
     lightning->SetVisible(false);
    
}

void PlaneActor::FieldMove(const InputState &state)
{
    float rightSpeed = 0.0f;
    float upSpeed = 0.0f;
    
    

    float speed = 80;

    
    
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_UP) == EHeld)
    {
        if(GetPosition().y < AREA_LIMIT_H) upSpeed += speed;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_DOWN) == EHeld)
    {
        if(GetPosition().y > -AREA_LIMIT_H) upSpeed -= speed;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_LEFT) == EHeld)
    {
        if(GetPosition().x > -AREA_LIMIT_W) rightSpeed -= speed;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_RIGHT) == EHeld)
    {
        if(GetPosition().x < AREA_LIMIT_W) rightSpeed += speed;
    }
    
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_Z) == EPressed)
    {
        ShotLaser();
    }
    
    
    moveComp->SetRightSpeed(rightSpeed);
    moveComp->SetUpSpeed(upSpeed);
      
      
}


void PlaneActor::ActorInput(const InputState &state)
{
    FieldMove(state);

}

void PlaneActor::UpdateActor(float deltaTime)
{
    collComp->SetDisp(true);
    auto v = GetPosition();
    scopeActor->SetPosition(Vector3(v.x, v.y, v.z+30));
    
    if(collComp->GetCollided())
    {
        for(auto col : collComp->GetTargetColliders())
        {
            if(col->GetColliderType() == C_BULLET || col->GetColliderType() == C_ENEMY)
            {
                barrierCnt = 0;
                DamageEffect(true);
                break;
            }
        }
    }
    barrierCnt++;
    if (barrierCnt > 15)
    {
        DamageEffect(false);
    }
    
}



void PlaneActor::SetMeshVisible(bool visible)
{
    meshComp->SetVisible(visible);
}

void PlaneActor::ShotLaser()
{
    for (int i = 0; i < MAX_LASER; i++)
    {
        if(!laserActor[i]->GetDisp())
        {
            laserActor[i]->Appear(GetPosition(), 0);
            break;
        }
    }
}

void PlaneActor::DamageEffect(bool b)
{
    if (b)
    {
        lightning->SetVisible(true);
        meshComp->SetGlory(true);

    }
    else
    {
        lightning->SetVisible(false);
        meshComp->SetGlory(false);

    }
}
