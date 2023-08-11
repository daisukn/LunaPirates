#include "UIElement.h"

#include "Application.h"
#include "Renderer.h"
#include "Actor.h"
#include "SpriteComponent.h"

UIElement::UIElement(Application* a)
    : score(0)
    , playerLife(0)
    , playerMaxLife(0)
    , stageNum(0)
    , time(0)
{
    gaugeActor = std::make_unique<Actor>(a);
    gaugeActor->SetPosition(Vector3(-500, 300, 0));
    gauge = std::make_unique<SpriteComponent>(gaugeActor.get());
    gauge->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/gauge.png"));

    
    gaugeFrameActor = std::make_unique<Actor>(a);
    gaugeFrameActor->SetPosition(Vector3(-500, 300, 0));
    gaugeFrame = std::make_unique<SpriteComponent>(gaugeFrameActor.get(), 100, false);
    gaugeFrame->SetTexture(a->GetRenderer()->GetTexture("Assets/Textures/gauge_frame.png"));
    
}

UIElement::~UIElement()
{
}


void UIElement::Update()
{
    float sc = static_cast<float>(playerLife) / static_cast<float>(playerMaxLife);
    gauge->SetScale(sc, 1.0f);
    float width = static_cast<float>(gauge->GetTexWidth());
    gaugeActor->SetPosition(Vector3(-500.f - (width -  width * sc) /2, 300, 0));
    
}
