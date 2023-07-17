#include "DragonActor.h"
#include "MeshComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "SkeletalMeshComponent.h"
#include "Mesh.h"
#include "MoveComponent.h"



DragonActor::DragonActor(Application* app)
    : Actor(app)
    , animID(0)
    , prevPos(Vector3::Zero)
{
    // メッシュ初期化
    meshComp = new SkeletalMeshComponent(this);
    meshComp->SetMesh(app->GetRenderer()->GetMesh("Assets/Dragon.fbx"));
    meshComp->SetAnimID(0, PLAY_CYCLIC);
    
    meshComp->SetToonRender(true, 1.003f);
    meshComp->SetVisible(true);
    
    
    // 場所調整
    SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    SetScale(0.10f);
    
    
    // 移動コンポーネント
    moveComp = new MoveComponent(this);
    

}
    



void DragonActor::ApplyMotion()
{

    meshComp->SetAnimID(animID, PLAY_CYCLIC);

}



void DragonActor::UpdateActor(float deltaTime) {

 
}



void DragonActor::SetVisible(bool visible)
{
    meshComp->SetVisible(visible);
}


