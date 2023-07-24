#include "TitleStage.h"
#include "Application.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "SpriteComponent.h"

TitleStage::TitleStage(class Application* a)
    : Stage(a)
{
    app->GetRenderer()->SetClearColor(0.5, 0.5, 0.5);
    LoadStageData();
}

TitleStage::~TitleStage()
{
    UnloadStageData();
}

float ang  = 0.0f;

void TitleStage::LoadStageData()
{
    moonActor = std::make_unique<Actor>(app);
    moonActor->SetPosition(Vector3(10, 5, 30));
    moonActor->SetScale(0.05f);

    moonMesh = std::make_unique<MeshComponent>(moonActor.get());
    moonMesh->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/moon.lwo"));
    

    planeActor = std::make_unique<Actor>(app);
    planeActor->SetScale(0.9f);
    Quaternion rot = Quaternion(Vector3::UnitY, Math::ToRadians(45));
    planeActor->SetRotation(rot);
    planeActor->SetPosition(Vector3(-5, -5, 30));

    planeMesh = std::make_unique<SkeletalMeshComponent>(planeActor.get());
    planeMesh->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/plane.fbx"));
    planeMesh->SetAnimID(0, PLAY_CYCLIC);
    
    
    skyActor = std::make_unique<Actor>(app);
    skySprite = std::make_unique<SpriteComponent>(skyActor.get(), 100, true);
    skySprite->SetTexture(app->GetRenderer()->GetTexture("Assets/Textures/night_sky.jpg"));
    
    
    
    isQuitStage = false;
    
}

void TitleStage::UnloadStageData()
{
//    delete titleActor;
//    app->RemoveActor(titleActor);
    app->GetRenderer()->UnloadData();
    //std::cout << "Unload StageData()" << std::endl;
}

void TitleStage::StageInput(const struct InputState &state)
{
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_RETURN) == EReleased)
    {
        isQuitStage = true;
    }
}

void TitleStage::UpdateStage()
{
    ang -= 0.1f;
    if (ang > 360.0f) ang = 0.0f;
    Quaternion rot = Quaternion(Vector3::UnitY, Math::ToRadians(ang));
    moonActor->SetRotation(rot);

}
