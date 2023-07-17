#ifndef __CLOUDSTAGE_H
#define __CLOUDSTAGE_H

#include "Stage.h"
#include "UfoEnemy.h"
#include "MoaiEnemy.h"
#include "LaserActor.h"
#include <memory>
#include <vector>


class CloudStage : public Stage
{
private:
    std::unique_ptr<class PlaneActor> planeActor;
    std::unique_ptr<class DragonActor> dragonActor;

    std::vector<std::unique_ptr<class CloudBGActor>> cloudActor;
    std::vector<std::unique_ptr<class ObjectActor>> ufoEnemy;
    std::vector<std::unique_ptr<class ObjectActor>> moaiEnemy;
    std::vector<std::unique_ptr<class ObjectActor>> shipEnemy;
    std::vector<std::unique_ptr<class ObjectActor>> laserActor;

    std::unique_ptr<class Actor> skyActor;
    std::unique_ptr<class MeshComponent> skyMesh;
    
    float ang = 0.f;
    
    unsigned int stageCounter;
    
    void GenerateCloud();
    

public:
    CloudStage(class Application* a);
    ~CloudStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;
    
    virtual void InputAction_A() override;
    
};


#endif // __CLOUDSTAGE_H
