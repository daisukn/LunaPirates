#include "StageObjectActor.h"

StageObjectActor::StageObjectActor(Application* a)
    : Actor(a)
    , isDisp(false)
{
    
}

void StageObjectActor::UpdateActor(float deltaTime)
{
    
}

void StageObjectActor::Appear(Vector3 pos)
{
    SetDisp(true);
    SetPosition(pos);
}
