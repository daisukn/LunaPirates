#ifndef __DRAGONACTOR_H
#define __DRAGONACTOR_H


#include "Actor.h"


// 飛行機
class DragonActor : public Actor
{
public:
    DragonActor(class Application* app);
    void UpdateActor(float deltaTime) override;
    void SetVisible(bool visible);
    
    

private:
    //  前フレームの場所
    Vector3 prevPos;
    
    // モーションID
    int animID;
    
    class SkeletalMeshComponent* meshComp;
    class MoveComponent* moveComp;
    
    
    // モーションを適用
    void ApplyMotion();
    


};



#endif // __DRAGONACTOR_H
