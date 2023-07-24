#pragma once

#include "Actor.h"
#include <memory>


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
    
    std::unique_ptr<class SkeletalMeshComponent> meshComp;
    
    // モーションを適用
    void ApplyMotion();
    


};

