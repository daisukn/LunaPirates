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
//    cnt++;
//    std::cout << "Component num = " << cnt << std::endl;
}

Component::~Component()
{
    owner->RemoveComponent(this);
//    cnt--;
//    std::cout << "Component num = " << cnt << std::endl;

}

void Component::Update(float deltaTime)
{
}

Vector3 Component::GetPosition() const
{
    return owner->GetPosition();
}
