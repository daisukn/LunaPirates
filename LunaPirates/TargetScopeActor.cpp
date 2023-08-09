#include "TargetScopeActor.h"
#include "BillboardComponent.h"
#include "Application.h"
#include "Renderer.h"
#include "Mesh.h"


TargetScopeActor::TargetScopeActor(Application* a, Stage* s)
    : StageObjectActor(a, s)
{
    isDisp = true;
    scopeBillboard = std::make_unique<BillboardComponent>(this, 200);
    scopeBillboard->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/target_scope.png"));
    scopeBillboard->SetVisible(true);
    SetPosition(Vector3(0, 0, 100));
    SetScale(0.1f);
}

void TargetScopeActor::UpdateActor(float deltaTime)
{

}
