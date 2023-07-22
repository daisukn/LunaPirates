#ifndef __STAGE_H
#define __STAGE_H

#include "Math.h"
#include <memory>
#include <vector>
#include <string>


struct StageLayout
{
    int frame;
    int objType;
    int behaveType;
    float x;
    float y;
    float z;
};


class Stage
{
public:
    Stage(class Application* a);
    virtual ~Stage();
    
    virtual void LoadStageData();
    virtual void UpdateStage();
    virtual void UnloadStageData();

    void ProcessInput(const struct InputState& state);
    // 派生先での入力処理 Override
    virtual void StageInput(const struct InputState& state);
    
    bool GetQuitStage() const { return isQuitStage; }
    
    
protected:
    class Application* app;
    
    bool isQuitStage; // trueになったら遷移する
    
    std::vector<struct StageLayout> layout;
    void LoadStageLayout(std::string filename);
    virtual void AppearLayout();
    int cntLayout;
    int numLayout;
    

private:
};


#endif // __STAGE_H
