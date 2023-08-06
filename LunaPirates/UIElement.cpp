#include "UIElement.h"

#include "Application.h"
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"

UIElement::UIElement(Application* a)
    : score(0)
    , playerLife(0)
    , stageNum(0)
    , time(0)
{
    gaugeActor = std::make_unique<Actor>(a);
    gauge = std::make_unique<SpriteComponent>(gaugeActor.get());
    gauge->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/gauge.png"));
    
    gaugeActor->SetPosition(Vector3(-500, 300, 0));

    gaugeFrame = std::make_unique<SpriteComponent>(gaugeActor.get(), 100, false);
    gaugeFrame->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/gauge_frame.png"));
    
}

UIElement::~UIElement()
{
    
}


void UIElement::Update()
{
}
