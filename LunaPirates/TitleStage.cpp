#include "TitleStage.h"
#include "Application.h"
#include "MeshComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "InputSystem.h"

TitleStage::TitleStage(class Application* a)
    : Stage(a)
{
    LoadStageData();
}

TitleStage::~TitleStage()
{
    UnloadStageData();
}

float ang  = 0.0f;

void TitleStage::LoadStageData()
{
    titleActor = std::make_unique<Actor>(app);
    titleActor->SetPosition(Vector3(0, 0, 10));

    mesh = std::make_unique<MeshComponent>(titleActor.get());
    mesh->SetMesh(app->GetRenderer()->GetMesh("Assets/title/title.lwo"));
    isQuitStage = false;
    
}

void TitleStage::UnloadStageData()
{
//    delete titleActor;
//    app->RemoveActor(titleActor);
    app->GetRenderer()->UnloadData();
    std::cout << "Unload StageData()" << std::endl;
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
    ang += 2.0f;
    Quaternion rot = Quaternion(Vector3::UnitY, Math::ToRadians(ang));
    titleActor->SetRotation(rot);

}
