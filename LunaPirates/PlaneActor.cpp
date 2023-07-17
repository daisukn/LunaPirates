
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


PlaneActor::PlaneActor(Application* app)
    : Actor(app)
    , animID(0)
    , isMovable(true)
    , prevPos(Vector3::Zero)
{
    // メッシュ初期化
    meshComp = std::make_unique<SkeletalMeshComponent>(this);
    meshComp->SetMesh(app->GetRenderer()->GetMesh("Assets/plane.fbx"));
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
    collComp->GetBoundingVolume()->ComputeBoundingVolume(app->GetRenderer()->GetMesh("Assets/plane.fbx")->GetVertexArray());
    collComp->GetBoundingVolume()->AdjustBoundingBox(Vector3(0, 0, 0), Vector3(1, 0.5, 1));
    collComp->GetBoundingVolume()->CreateVArray();
    collComp->GetBoundingVolume()->SetVisible(true);

    
//    collComp->GetBoundingVolume()->ComputeBoundingVolume(Vector3(150, 150, 400), Vector3(300, 300, 800));
//    collComp->GetBoundingVolume()->CreateVArray();
    
}

void PlaneActor::FieldMove(const InputState &state)
{
    float rightSpeed = 0.0f;
    float upSpeed = 0.0f;
    
    

    float speed = 50;

    
    
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_UP) == EHeld)
    {
        if(GetPosition().y < 40) upSpeed += speed;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_DOWN) == EHeld)
    {
        if(GetPosition().y > -40) upSpeed -= speed;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_LEFT) == EHeld)
    {
        if(GetPosition().x > -80) rightSpeed -= speed;
    }
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_RIGHT) == EHeld)
    {
        if(GetPosition().x < 80) rightSpeed += speed;
    }
    
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_Z) == EPressed)
    {
        ownerStage->InputAction_A();
    }
    
    
    moveComp->SetRightSpeed(rightSpeed);
    moveComp->SetUpSpeed(upSpeed);
      
      
}


void PlaneActor::ApplyMotion()
{

    meshComp->SetAnimID(animID, PLAY_CYCLIC);

}


void PlaneActor::ActorInput(const InputState &state)
{
    FieldMove(state);

}

void PlaneActor::UpdateActor(float deltaTime)
{

}



void PlaneActor::SetMeshVisible(bool visible)
{
    meshComp->SetVisible(visible);
}


