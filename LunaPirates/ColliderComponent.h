#ifndef __COMPONENT_H
#define __COMPONENT_H


#include "Component.h"

#include <vector>

// 通知される相手のタイプ
enum ColliderType
{
    C_NONE,     // 
    C_PLAYER,   // プレーヤー
    C_ENEMY,    // 敵
    C_ATTACK,
    C_BULLET,   // 弾丸
    C_OBSTACLE  // 障害物
};

class ColliderComponent : public Component
{
public:
    ColliderComponent(class Actor* a);
    ~ColliderComponent();

    // 自分のタイプ
    void SetColliderType(const ColliderType t) { type = t; }
    ColliderType GetColliderType() const { return type; }
    // 衝突した
    void Collided(ColliderComponent* c) { targetColliders.emplace_back(c); }
    std::vector<ColliderComponent*> GetTargetColliders() const { return targetColliders; }
    void ClearCollidBuffer() { targetColliders.clear(); }
    
    void Update(float deltaTime) override;
    
    class Actor* GetActor() { return owner; }
    class BoundingVolumeComponent* GetBoundingVolume() const { return boundingVolume; }
    
    

    
private:
    class BoundingVolumeComponent* boundingVolume;
    
    ColliderType type;
    std::vector<ColliderComponent*> targetColliders;
    
};

#endif // __COMPONENT_H
