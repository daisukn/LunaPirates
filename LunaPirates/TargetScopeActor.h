#ifndef __TARGETSCOPEACTOR_H
#define __TARGETSCOPEACTOR_H

#include "StageObjectActor.h"
#include "BillboardComponent.h"
#include <memory>


class TargetScopeActor : public StageObjectActor
{
public:
    TargetScopeActor(class Application* a);
    void UpdateActor(float deltaTime) override;
private:
    std::unique_ptr<class BillboardComponent> scopeBillboard;
};


#endif // __TARGETSCOPEACTOR_H
