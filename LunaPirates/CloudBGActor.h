#ifndef __BGCLOUDACTOR_H
#define __BGCLOUDACTOR_H

#include "Actor.h"

#include "BillboardComponent.h"
#include "MoveComponent.h"
#include "FollowCamera.h"
#include <memory>


// 背景用雲
class CloudBGActor : public Actor
{
public:
    CloudBGActor(class Application* a);

    void UpdateActor(float deltaTime) override;

    //void SetVisible(bool visible);
    //bool GetVisible() const { return isVisible; }
    
    bool GetDisp() const { return isDisp; }
    void SetDisp(bool b) { isDisp = b; }
    
private:
    //  前フレームの場所
    Vector3 prevPos;
    
    std::unique_ptr<class MoveComponent> moveComp;
    
    std::unique_ptr<class BillboardComponent> cloudBillboard;
    
   
    bool isDisp;

};






#endif // __BGCLOUDACTOR_H
