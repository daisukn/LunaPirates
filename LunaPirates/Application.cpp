#include "Application.h"
#include "Actor.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "PhysWorld.h"

#include "TitleStage.h"
#include "CloudStage.h"
#include <algorithm>


const float SCREEN_W = 1600.f;
const float SCREEN_H = 900.f;

// コンストラクタ
Application::Application()
    : isActive(false)
    , isUpdatingActors(false)
    , activeStage(nullptr)
    , stageTransition(STAGE_TITLE)
{
    renderer = std::make_unique<class Renderer>();
    inputSys = std::make_unique<class InputSystem>();
    physWorld = std::make_unique<class PhysWorld>();

}

// デストラクタ
Application::~Application()
{
    delete activeStage;
}


// アプリ初期化
bool Application::Initialize()
{
    
    // SDL初期化
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Failed to init SDL\n";
        return false;
    }
    
    // Renderer初期化
    renderer->Initialize("Project Luna", SCREEN_W, SCREEN_H);

    // 入力システム初期化
    inputSys->Initialize();
    
    // データ ロード、主にRendererに登録されるもの。
    LoadData();
    
    isActive = true;
    ticksCount = SDL_GetTicks();
    return true;
}


// メインループ
void Application::RunLoop()
{
    while (isActive)
    {
        ProcessInput();
        UpdateGame();
        Draw();
    }
}

// 描画処理 Rendererが描画する
void Application::Draw()
{
    renderer->Draw();
}


// 終了処理
void Application::Shutdown()
{
    UnloadData();
    inputSys->Shutdown();
    renderer->Shutdown();
    SDL_Quit();
}

// 入力受付
void Application::ProcessInput(){
    
    // 前の状態を保存
    inputSys->PrepareForUpdate();
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
                
        case SDL_QUIT:
            isActive = false;
            break;
        }
    }

    inputSys->Update();
    const InputState& state = inputSys->GetState();
        
    if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EReleased)
    {
        isActive = false;
    }

    //UpdatingActors = true;

    //UpdatingActors = false;
    
    
    // 配下のActorsに渡す
    for (auto actor : actors)
    {
        actor->ProcessInput(state);
    }
    // Stageに渡す
    activeStage->ProcessInput(state);
}


// Actor追加
void Application::AddActor(Actor* actor)
{
    // メインのActorsがUpdate中はPendingに追加
    if (isUpdatingActors)
    {
        pendingActors.emplace_back(actor);
    }
    else
    {
        actors.emplace_back(actor);
    }
}

// Actor削除
void Application::RemoveActor(Actor* actor)
{
    // Pendingにないか確認
    auto iter = std::find(pendingActors.begin(), pendingActors.end(), actor);
    if (iter != pendingActors.end())
    {
        std::iter_swap(iter, pendingActors.end() - 1);
        pendingActors.pop_back();
    }

    // actorsにある場合は削除
    iter = std::find(actors.begin(), actors.end(), actor);
    if (iter != actors.end() && !actors.empty())
    {
        std::iter_swap(iter, actors.end() - 1);
        actors.pop_back();
    }

}


// データ解放
void Application::UnloadData()
{
    actors.clear();
    /*
    while (!actors.empty())
    {
        delete actors.back();
        actors.pop_back();
    }
    */
    if (renderer)
    {
        renderer->UnloadData();
    }
}

// Actors, Renderer関連
void Application::LoadData()
{
    
    
    // ライト
    renderer->SetAmbientLight(Vector3(1.f, 1.f, 1.f));
    DirectionalLight& dir = renderer->GetDirectionalLight();
    dir.Direction = Vector3(0.f, 0.f, 1.f);
    dir.DiffuseColor = Vector3(0.4f, 0.4f, 0.4f);
    dir.SpecColor = Vector3(0.1f, 0.1f, 0.1f);
    
    // ステージ読み込み
    stageTransition = STAGE_TITLE;
    activeStage = new TitleStage(this);
}

// ゲームメインルーチン
void Application::UpdateGame()
{
    
    // FPS60固定
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    ticksCount = SDL_GetTicks();
    //std::cout << 1.0f / deltaTime << std::endl;

    // 物理計算系
    //physWorld->ComputeGroundHeight();
    //physWorld->Test();
    

    activeStage->UpdateStage();
    if(activeStage->GetQuitStage())
    {
        TransitionStage();
    }
    // Actorsメイン呼び出し
    isUpdatingActors = true;
    for (auto a : actors)
    {
        a->Update(deltaTime);
    }
    isUpdatingActors = false;

    // Pendingがある場合はActorsに移動
    for (auto p : pendingActors)
    {
        p->ComputeWorldTransform();
        actors.emplace_back(p);
    }
    pendingActors.clear();

    // EDeadフラグのアクターは削除
    std::vector<Actor*> deadActors;
    for (auto a : actors)
    {
        if (a->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(a);
        }
    }
    
    for (auto a : deadActors)
    {
        delete a;
    }

}

void Application::TransitionStage()
{

    //activeStage->UnloadStageData();
    delete activeStage;
    activeStage = nullptr;

    
    switch (stageTransition)
    {
        case STAGE_TITLE:
            //std::cout << "きたよ" << std::endl;
            stageTransition = STAGE_CLOUD;
            activeStage = new CloudStage(this);
            break;
    
        case STAGE_CLOUD:
            break;
        default:
            break;
    }
}
