#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

enum StageTransitionID
{
    STAGE_TITLE,
    STAGE_CLOUD,
    STAGE_END
};



// アプリ制御クラス
class Application
{
public:
    // コンストラクタ
    Application();
    // デストラクタ
    ~Application();

    // 初期化
    bool Initialize();
    // メインループ
    void RunLoop();
    // 終了処理
    void Shutdown();
    
    // アクターを登録/削除
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    
    // 描画エンジンを取得
    class Renderer* GetRenderer() const { return renderer.get(); }
    // 物理エンジンを取得
    class PhysWorld* GetPhysWorld() const { return physWorld.get(); }
    

    
    
private:
    
    // ゲームデータ読み込み/開放
    void UnloadData();
    void LoadData();
    
    // false 終了、true 実行中
    bool isActive;
    // 描画エンジン
    std::unique_ptr<class Renderer> renderer;
    // 入力エンジン
    std::unique_ptr<class InputSystem> inputSys;
    // 物理エンジン
    std::unique_ptr<class PhysWorld> physWorld;
    // 時間管理
    unsigned int ticksCount;

    
    // アクティブなアクター
    std::vector<class Actor*> actors;
    // ペンディング中のアクター
    std::vector<class Actor*> pendingActors;
    // true の場合Pendingsにまわす
    bool isUpdatingActors;

    // カメラ
    //class CameraActor* cameraAct;

    // ステージデータ
    class Stage* activeStage;
    int activeStageID;
    StageTransitionID stageTransition;
    void TransitionStage();

    // 入力受付
    void ProcessInput();
    // ゲームメイン
    void UpdateGame();
    // 描画
    void Draw();


};


#endif // __APPLICATION_H
