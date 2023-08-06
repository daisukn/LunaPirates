#pragma once

#include "Stage.h"
#include "UfoEnemy.h"
#include "MoaiEnemy.h"
#include "UIElement.h"
#include <memory>
#include <vector>


class CloudStage : public Stage
{
public:
    CloudStage(class Application* a);
    ~CloudStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;

    
    
private:
    std::unique_ptr<class PlaneActor> planeActor;
    std::unique_ptr<class DragonActor> dragonActor;

    std::vector<std::unique_ptr<class CloudBGActor>> cloudActor;
    std::vector<std::unique_ptr<class StageObjectActor>> ufoEnemy;
    std::vector<std::unique_ptr<class StageObjectActor>> moaiEnemy;
    std::vector<std::unique_ptr<class StageObjectActor>> shipEnemy;

    

    std::unique_ptr<class Actor> skyActor;
    std::unique_ptr<class MeshComponent> skyMesh;
    
    std::unique_ptr<class UIElement> ui;
    
    float ang = 0.f;
    
    unsigned int stageCounter;
    
    void GenerateCloud();
    void AppearLayout() override;
    void AppearRandom();
    void DebugAppear();


        
};

