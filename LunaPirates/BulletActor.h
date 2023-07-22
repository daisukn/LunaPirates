#ifndef __BULLETACTOR_H
#define __BULLETACTOR_H

#include "StageObjectActor.h"
#include "BillboardComponent.h"

class BulletActor : public StageObjectActor
{
public:
    BulletActor(class Application* a);
    void UpdateActor(float deltaTime) override;
    void SetAngle(float f) { angle = f; }
    float GetAngle() const { return angle; }
private:
    std::unique_ptr<class BillboardComponent> flare;
    float angle;
};


#endif // __BULLETACTOR_H
