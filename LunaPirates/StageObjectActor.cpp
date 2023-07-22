#include "StageObjectActor.h"

StageObjectActor::StageObjectActor(Application* a)
    : Actor(a)
    , isDisp(false)
    , state(StateNormal)
{
    
}

void StageObjectActor::UpdateActor(float deltaTime)
{
    
}

void StageObjectActor::Appear(Vector3 pos, int type)
{
    behaveType = type;
    state = StateNormal;
    SetDisp(true);
    SetPosition(pos);
}
