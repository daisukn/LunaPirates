#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Application.h"
#include "ColliderComponent.h"

CameraComponent::CameraComponent(Actor* a, int updateOrder)
    : Component(a, updateOrder)
{
    cameraActor = std::make_unique<Actor>(owner->GetApp());
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	Application* app = owner->GetApp();
	app->GetRenderer()->SetViewMatrix(view);
}

void CameraComponent::Update(float deltaTime)
{
    Matrix4 inView = owner->GetApp()->GetRenderer()->GetInvViewMatrix();
    CameraPosition = inView.GetTranslation();
}


