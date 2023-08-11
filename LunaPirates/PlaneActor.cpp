
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
#include "SpriteComponent.h"

const int MAX_LASER = 20;

const float AREA_LIMIT_H = 45.f;
const float AREA_LIMIT_W = 75.f;

PlaneActor::PlaneActor(Application* a, Stage* s)
    : StageObjectActor(a, s)
    , animID(0)
    , isMovable(true)
    , barrierCnt(0)
    , maxLife(100)
    , life(100)
 {
     // メッシュ初期化
     meshComp = std::make_unique<SkeletalMeshComponent>(this);
     meshComp->SetMesh(a->GetRenderer()->GetMesh("Assets/Models/plane.fbx"));
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
     collComp->GetBoundingVolume()->ComputeBoundingVolume(a->GetRenderer()->GetMesh("Assets/Models/plane.fbx")->GetVertexArray());
     collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 0.5, 1));
     collComp->GetBoundingVolume()->CreateVArray();
     collComp->GetBoundingVolume()->SetVisible(true);

    
     // レーザー
     for (int i = 0; i < MAX_LASER; i++)
     {
         laserActor.emplace_back( std::make_unique<LaserActor>(a, ownerStage));
     }
     

     // ターゲットスコープ
     scopeActor = std::make_unique<TargetScopeActor>(a, ownerStage);
     scopeActor->SetOwnerStage(ownerStage);
     scopeActor->SetDisp(true);
     

     // 稲妻
     lightning = std::make_unique<MeshComponent>(this, false, MESH_EFFECT);
     lightning->SetMesh(a->GetRenderer()->GetMesh("Assets/Models/lightning2.lwo"));
     lightning->SetBlendAdd(true);
     lightning->SetVisible(false);
}

void PlaneActor::FieldMove(const InputState &state)
{
    float rightSpeed = 0.0f;
    float upSpeed = 0.0f;
    
    float speed = 80;

    upSpeed = speed * state.Controller.GetLeftStick().y;
    rightSpeed = speed * state.Controller.GetLeftStick().x;
    
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
        
    
    moveComp->SetRightSpeed(rightSpeed);
    moveComp->SetUpSpeed(upSpeed);
      
      
}


void PlaneActor::ActorInput(const InputState &state)
{
    FieldMove(state);
    
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_Z) == EPressed ||
        state.Controller.GetButtonState(SDL_CONTROLLER_BUTTON_A) == EPressed )
    {
        ShotLaser();
    }
}

void PlaneActor::UpdateActor(float deltaTime)
{
    collComp->SetDisp(true);
    auto v = GetPosition();
    scopeActor->SetPosition(Vector3(v.x, v.y, v.z+30));
    
    if (collComp->GetCollided())
    {
        for (auto col : collComp->GetTargetColliders())
        {
            if (col->GetColliderType() == C_BULLET || col->GetColliderType() == C_ENEMY)
            {
                DamageEffect(true);
                life--;
                break;
            }
        }
    }
    barrierCnt--;
    if (barrierCnt == 0)
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
        if (!laserActor[i]->GetDisp())
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
        barrierCnt = 15;

    }
    else
    {
        lightning->SetVisible(false);
        meshComp->SetGlory(false);

    }
}
