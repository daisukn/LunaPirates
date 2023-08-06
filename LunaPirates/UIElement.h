#pragma once

#include "SpriteComponent.h"
#include "Actor.h"
#include <memory>

class UIElement
{
public:
    UIElement(class Application* a);
    ~UIElement();
    
    void SetLife(int life) { playerLife = life; }
    
    void Update();

private:
    int score;
    int playerLife;
    int stageNum;
    int time;
    
    std::unique_ptr<class Actor> gaugeActor;
    std::unique_ptr<class SpriteComponent> gauge;
    std::unique_ptr<class SpriteComponent> gaugeFrame;
};
