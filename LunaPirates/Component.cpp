#include "Component.h"
#include "Actor.h"
#include <iostream>

//static int cnt = 0;

Component::Component(Actor* a, int order)
    : owner(a)
    , updateOrder(order)
{
	// アクターに持たせる
	owner->AddComponent(this);
}

Component::~Component()
{
    owner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}

Vector3 Component::GetPosition() const
{
    return owner->GetPosition();
}
