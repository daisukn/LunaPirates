#pragma once

#include "Stage.h"
#include "SpriteComponent.h"
#include <memory>


class TitleStage : public Stage
{
    std::unique_ptr<class Actor> moonActor;
    std::unique_ptr<class Actor> planeActor;
    std::unique_ptr<class Actor> skyActor;
    std::unique_ptr<class MeshComponent> moonMesh;
    std::unique_ptr<class SkeletalMeshComponent> planeMesh;
    std::unique_ptr<class SpriteComponent> skySprite;
public:
    TitleStage(class Application* a);
    virtual ~TitleStage();
    
    virtual void LoadStageData() override;
    virtual void UnloadStageData() override;
    virtual void UpdateStage() override;
    
    virtual void StageInput(const struct InputState& state) override;
};

