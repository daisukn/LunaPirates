#include "Stage.h"
#include "Application.h"

Stage::Stage(class Application* a)
    : app(a)
    , isQuitStage(false)
{
    
}

Stage::~Stage()
{
    UnloadStageData();
}

void Stage::LoadStageData()
{
    // overrideする
}

void Stage::UnloadStageData()
{
    // overrideする
}

void Stage::UpdateStage()
{
    // overrideする
}

void Stage::ProcessInput(const struct InputState &state)
{
    StageInput(state);
}

void Stage::StageInput(const struct InputState &state)
{
    
}
