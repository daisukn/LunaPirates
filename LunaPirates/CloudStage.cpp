#include "CloudStage.h"
#include "Application.h"
#include "MeshComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "PlaneActor.h"
#include "DragonActor.h"
#include "CloudBGActor.h"
#include "UfoEnemy.h"
#include "ShipEnemy.h"
#include "LaserActor.h"

#include <cstdlib>
#include <ctime>

const int MAX_CLOUD = 50;
const int MAX_UFO = 20;
const int MAX_MOAI = 10;
const int MAX_SHIP = 5;

const float APEAR_POINT = 1000.f;

CloudStage::CloudStage(class Application* a)
    : Stage(a)
    , stageCounter(0)
{
    LoadStageData();
    std::srand((unsigned int)std::time(nullptr));
    
}

CloudStage::~CloudStage()
{
    UnloadStageData();
}


void CloudStage::LoadStageData()
{
    
    planeActor = std::make_unique<PlaneActor>(app);
    planeActor->SetPosition(Vector3(0, 0, 30));
    planeActor->SetOwnerStage(this);

    dragonActor = std::make_unique<DragonActor>(app);
    dragonActor->SetPosition(Vector3(0, 0, 100));
    dragonActor->SetVisible(false);

    
    for(int i = 0; i < MAX_CLOUD; i++)
    {
        cloudActor.push_back( std::make_unique<CloudBGActor>(app) );
    }
    for(int i = 0;  i < MAX_UFO; i++)
    {
        ufoEnemy.push_back( std::make_unique<UfoEnemy>(app));
    }
    for(int i = 0;  i < MAX_MOAI; i++)
    {
        moaiEnemy.push_back( std::make_unique<MoaiEnemy>(app));
    }
    for(int i = 0;  i < MAX_SHIP; i++)
    {
        shipEnemy.push_back( std::make_unique<ShipEnemy>(app));
    }
    
    

    skyActor = std::make_unique<Actor>(app);
    skyActor->SetPosition(Vector3(0, 0, 500));
    Quaternion q(Vector3::UnitY, Math::ToRadians(180));
    skyActor->SetRotation(q);
    skyActor->SetScale(1);
    skyMesh = std::make_unique<MeshComponent>(skyActor.get(), false, true);
    skyMesh->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/sky.lwo"));
    

    isQuitStage = false;
    
    app->GetRenderer()->SetClearColor(0.596f, 0.733f, 0.858f);
    //app->GetRenderer()->SetClearColor(0.05f, 0.01f, 0.258f);
    
    
    LoadStageLayout("Setting/Stage1.txt");
/*    for(auto l : layout)
    {
        std::cout << l.frame << " " << l.x << " " << l.y << " " << l.z << " " << l.objType << " " << l.behaveType << std::endl;
    }
 */
}

void CloudStage::UnloadStageData()
{
}

void CloudStage::StageInput(const struct InputState &state)
{
    
}


void CloudStage::UpdateStage()
{
    stageCounter++;
    GenerateCloud();

    AppearLayout();
    //AppearRandom();
    
}

void CloudStage::GenerateCloud()
{
    if(stageCounter % 2 == 0)
    {
        for(int i = 0; i < MAX_CLOUD; i++)
        {
            if(!cloudActor[i]->GetDisp())
            {
                cloudActor[i]->SetDisp(true);
                cloudActor[i]->SetPosition(Vector3(std::rand() % 600 - 300, std::rand() % 400 - 200 , APEAR_POINT));
                break;
            }
        }
    }
}


void CloudStage::AppearLayout()
{

    if(numLayout > stageCounter || cntLayout > numLayout)
    {
        return;
    }
    
    
    while(layout[cntLayout].frame == stageCounter)
    {
        switch(layout[cntLayout].objType)
        {
            case 1: // UFO
                for(int i = 0; i < MAX_UFO; i++)
                {
                    if(!ufoEnemy[i]->GetDisp())
                    {
                        ufoEnemy[i]->Appear(Vector3(layout[cntLayout].x, layout[cntLayout].y ,layout[cntLayout].z), layout[cntLayout].behaveType);
                        break;
                    }
                }
                break;
            case 2: // moai
                for(int i = 0; i < MAX_MOAI; i++)
                {
                    if(!moaiEnemy[i]->GetDisp())
                    {
                        moaiEnemy[i]->Appear(Vector3(layout[cntLayout].x, layout[cntLayout].y ,layout[cntLayout].z), layout[cntLayout].behaveType);
                        break;
                    }
                }
                break;
            case 3: // ship
                for(int i = 0; i < MAX_SHIP; i++)
                {
                    if(!shipEnemy[i]->GetDisp())
                    {
                        shipEnemy[i]->Appear(Vector3(layout[cntLayout].x, layout[cntLayout].y ,layout[cntLayout].z), layout[cntLayout].behaveType);
                        break;
                    }
                }
                break;
        }
        cntLayout++;
    }

}


void CloudStage::AppearRandom()
{
    if(stageCounter % 40 == 0)
    {
        for(int i = 0; i < MAX_UFO; i++)
        {
            if(!ufoEnemy[i]->GetDisp())
            {
                ufoEnemy[i]->Appear(Vector3(std::rand() % 180 - 90, std::rand() % 90 - 45 , APEAR_POINT), 0);
                break;
            }
        }
    }
    
    if(stageCounter % 60 == 0)
    {
        for(int i = 0; i < MAX_MOAI; i++)
        {
            if(!moaiEnemy[i]->GetDisp())
            {
                moaiEnemy[i]->Appear(Vector3(std::rand() % 180 - 90, std::rand() % 90 - 45 , APEAR_POINT), 0);
                break;
            }
        }
    }
    
    if(stageCounter % 60 == 0)
    {
        for(int i = 0; i < MAX_SHIP; i++)
        {
            if(!shipEnemy[i]->GetDisp())
            {
                shipEnemy[i]->Appear(Vector3(std::rand() % 180 - 90, std::rand() % 90 - 45 , APEAR_POINT), 0);
                break;
            }
        }
    }
}

