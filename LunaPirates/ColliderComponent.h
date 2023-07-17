#ifndef __COMPONENT_H
#define __COMPONENT_H


#include "Component.h"

#include <vector>
#include <memory>

// 通知される相手のタイプ
enum ColliderType
{
    C_NONE,     // 
    C_PLAYER,   // プレーヤー
    C_ENEMY,    // 敵
    C_BULLET,   // 弾丸
    C_LASER
};

class ColliderComponent : public Component
{
public:
    ColliderComponent(class Actor* a);
    virtual ~ColliderComponent();

    // 自分のタイプ
    void SetColliderType(const ColliderType t);
    ColliderType GetColliderType() const { return type; }
    
    // 衝突した
    void Collided(ColliderComponent* c);
    
    std::vector<ColliderComponent*> GetTargetColliders() const { return targetColliders; }
    void ClearCollidBuffer() { targetColliders.clear(); }
    
    void Update(float deltaTime) override;
    
    class Actor* GetActor() const { return owner; }
    class BoundingVolumeComponent* GetBoundingVolume() const { return boundingVolume.get(); }
    
    // 衝突しているかどうか
    bool GetCollided() const { return isCollided; }
    void SetCollided(bool b) { isCollided = b; }
    
    // 表示されているかどうか
    bool GetDisp() const { return isDisp; }
    void SetDisp(bool b) { isDisp = b; }

    
private:
    
    bool isCollided;
    bool isDisp;
    
    std::unique_ptr<class BoundingVolumeComponent> boundingVolume;
    
    ColliderType type;
    std::vector<ColliderComponent*> targetColliders;
    
};

#endif // __COMPONENT_H
