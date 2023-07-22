#include "ColliderComponent.h"
#include "Actor.h"
#include "BoundingVolumeComponent.h"
#include "Application.h"
#include "PhysWorld.h"

// コンストラクタ
ColliderComponent::ColliderComponent(Actor* a)
    : Component(a)
    , type(C_NONE)
    , isCollided(false)
    , isDisp(false)
//, targetType(C_NONE)
{
    boundingVolume = std::make_unique<BoundingVolumeComponent>(a);
    owner->GetApp()->GetPhysWorld()->AddCollider(this);
}

ColliderComponent::~ColliderComponent()
{
    owner->GetApp()->GetPhysWorld()->RemoveCollider(this);
}

void ColliderComponent::Update(float deltaTime)
{
    //targetType.clear();
}


// 衝突した
void ColliderComponent::Collided(ColliderComponent* c)
{
    targetColliders.emplace_back(c);
    isCollided = true;
}


void ColliderComponent::SetColliderType(const ColliderType t)
{
    type = t;
    owner->GetApp()->GetPhysWorld()->AddColliderType(this, t);
    
}
