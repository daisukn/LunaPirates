#include "ColliderComponent.h"
#include "Actor.h"
#include "BoundingVolumeComponent.h"
#include "Application.h"
#include "PhysWorld.h"

// コンストラクタ
ColliderComponent::ColliderComponent(Actor* a)
    : Component(a)
    , type(C_NONE)
//, targetType(C_NONE)
{
    boundingVolume = new BoundingVolumeComponent(a);
    owner->GetApp()->GetPhysWorld()->AddCollider(this);
}

ColliderComponent::~ColliderComponent()
{
    //owner->GetApp()->GetPhysWorld()->RemoveCollider(this);
}

void ColliderComponent::Update(float deltaTime)
{
    //targetType.clear();
}
