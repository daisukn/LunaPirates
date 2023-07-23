#pragma once

#include "Stage.h"
#include <memory>

class TitleStage : public Stage
{
    std::unique_ptr<class Actor> titleActor;
    std::unique_ptr<class MeshComponent> mesh;
public:
    TitleStage(class Application* a);
    virtual ~TitleStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;
};

