#include "CloudBGActor.h"
#include "Application.h"
#include "Renderer.h"


CloudBGActor::CloudBGActor(Application* a)
    : Actor(a)
    , isDisp(false)
{
    cloudBillboard = std::make_unique<BillboardComponent>(this);
    cloudBillboard->SetTexture(GetApp()->GetRenderer()->GetTexture("Assets/Textures/cloud_1.png"));
    cloudBillboard->SetScale(1.3f);
}

void CloudBGActor::UpdateActor(float deltaTime)
{
    if (isDisp)
    {
        float speed = 250.0f * deltaTime;
        cloudBillboard->SetVisible(true);
        auto v = GetPosition();
        SetPosition(Vector3(v.x, v.y, v.z-speed));
        if(v.z < 0)
        {
            isDisp = false;
            cloudBillboard->SetVisible(false);
        }
    }
}
