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
const int MAX_UFO = 50;
const int MAX_MOAI = 10;
const int MAX_SHIP = 5;

const float APEAR_POINT = 600.f;
const std::string stageFileName = "Setting/stage_test.csv";


//#define RANDOM_DEBUG
//#define STAGE_DEBUG

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
    


    dragonActor = std::make_unique<DragonActor>(app);
    dragonActor->SetPosition(Vector3(0, 0, 100));
    dragonActor->SetVisible(false);

    
    for(int i = 0; i < MAX_CLOUD; i++)
    {
        cloudActor.emplace_back( std::make_unique<CloudBGActor>(app) );
    }
    for(int i = 0;  i < MAX_UFO; i++)
    {
        ufoEnemy.emplace_back( std::make_unique<UfoEnemy>(app));
    }
    for(int i = 0;  i < MAX_MOAI; i++)
    {
        moaiEnemy.emplace_back( std::make_unique<MoaiEnemy>(app));
    }
    for(int i = 0;  i < MAX_SHIP; i++)
    {
        shipEnemy.emplace_back( std::make_unique<ShipEnemy>(app));
    }
    
    

    skyActor = std::make_unique<Actor>(app);
    skyActor->SetPosition(Vector3(0, 0, 500));
    Quaternion q(Vector3::UnitY, Math::ToRadians(180));
    skyActor->SetRotation(q);
    skyActor->SetScale(1);
    skyMesh = std::make_unique<MeshComponent>(skyActor.get(), false, MESH_BG);
    skyMesh->SetMesh(app->GetRenderer()->GetMesh("Assets/Models/sky.lwo"));
    

    isQuitStage = false;
    
    app->GetRenderer()->SetClearColor(0.596f, 0.733f, 0.858f);
    
    
    
    // 飛行機
    
    planeActor = std::make_unique<PlaneActor>(app);
    planeActor->SetPosition(Vector3(0, 0, 30));
    planeActor->SetOwnerStage(this);
    
    LoadStageLayout(stageFileName);

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

#ifdef RANDOM_DEBUG
    AppearRandom();
#else
    AppearLayout();
#endif // RANDOM_DEBUG

#ifdef STAGE_DEBUG
    DebugAppear();
#endif //STAGE_DEBUG
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

    if(numLayout <= cntLayout)
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
        if (cntLayout >= layout.size())
        {
            break;
        }
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
                ufoEnemy[i]->Appear(Vector3(std::rand() % 120 - 60, std::rand() % 90 - 45 , 0), 0);
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
                moaiEnemy[i]->Appear(Vector3(std::rand() % 120 - 60, std::rand() % 90 - 45 , APEAR_POINT), 0);
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
                shipEnemy[i]->Appear(Vector3(std::rand() % 120 - 60, std::rand() % 90 - 45 , APEAR_POINT), 0);
                break;
            }
        }
    }
}

void CloudStage::DebugAppear()
{
    if(stageCounter % 500 == 0)
    {
        for(int i = 0; i < MAX_MOAI; i++)
        {
            if(!moaiEnemy[i]->GetDisp())
            {
                moaiEnemy[i]->Appear(Vector3(-50, 0 , APEAR_POINT), 0);
                break;
            }
        }
    }
    if(stageCounter % 500 == 250)
    {
        for(int i = 0; i < MAX_MOAI; i++)
        {
            if(!moaiEnemy[i]->GetDisp())
            {
                moaiEnemy[i]->Appear(Vector3(50, 0 , APEAR_POINT), 0);
                break;
            }
        }
    }
}
