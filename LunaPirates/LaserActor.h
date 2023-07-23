#pragma once

#include "StageObjectActor.h"

class LaserActor : public StageObjectActor
{
public:
    LaserActor(class Application* a);
    void UpdateActor(float deltaTime) override;
};


