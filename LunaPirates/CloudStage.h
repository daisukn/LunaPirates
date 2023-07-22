#ifndef __CLOUDSTAGE_H
#define __CLOUDSTAGE_H

#include "Stage.h"
#include "UfoEnemy.h"
#include "MoaiEnemy.h"
#include <memory>
#include <vector>


class CloudStage : public Stage
{
private:
    std::unique_ptr<class PlaneActor> planeActor;
    std::unique_ptr<class DragonActor> dragonActor;

    std::vector<std::unique_ptr<class CloudBGActor>> cloudActor;
    std::vector<std::unique_ptr<class StageObjectActor>> ufoEnemy;
    std::vector<std::unique_ptr<class StageObjectActor>> moaiEnemy;
    std::vector<std::unique_ptr<class StageObjectActor>> shipEnemy;

    

    std::unique_ptr<class Actor> skyActor;
    std::unique_ptr<class MeshComponent> skyMesh;
    
    float ang = 0.f;
    
    unsigned int stageCounter;
    
    void GenerateCloud();
    void AppearLayout() override;
    void AppearRandom();

public:
    CloudStage(class Application* a);
    ~CloudStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;
        
};


#endif // __CLOUDSTAGE_H
