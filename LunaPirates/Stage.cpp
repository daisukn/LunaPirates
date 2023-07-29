#include "Stage.h"
#include "Application.h"


#include <fstream>
#include <sstream>
#include <algorithm>

Stage::Stage(class Application* a)
    : app(a)
    , isQuitStage(false)
    , numLayout(0)
    , cntLayout(0)
{
    
}

Stage::~Stage()
{
    UnloadStageData();
}

void Stage::LoadStageData()
{
    // overrideする
}

void Stage::LoadStageLayout(std::string filename)
{
    // 一旦クリア
    layout.clear();
    numLayout = 0;
    cntLayout = 0;
    
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line))
    {
        // 空白行と、#で始まる行は読み飛ばす
        if (line.empty())
        {
            continue;
        }
        if (line[0] == '#' || line[0] == ',')
        //if (!std::isdigit(line[0]))
        {
            continue;
        }
        std::istringstream ss(line);
        std::string frame;
        std::string x;
        std::string y;
        std::string z;
        std::string objType;
        std::string behaveType;
        
        // カンマで分割
        std::getline(ss, frame, ',');
        std::getline(ss, objType, ',');
        std::getline(ss, behaveType, ',');
        std::getline(ss, x, ',');
        std::getline(ss, y, ',');
        std::getline(ss, z, ',');
        
        // データ変換
        StageLayout sl;
        sl.frame = std::stoi(frame);
        sl.objType = std::stoi(objType);
        sl.behaveType = std::stoi(behaveType);
        sl.x = std::stof(x);
        sl.y = std::stof(y);
        sl.z = std::stof(z);
        
        layout.emplace_back(sl);
        
    }
    
    // フレームでソート
    std::sort(layout.begin(), layout.end(), [](const StageLayout& a, const StageLayout& b){
         return a.frame < b.frame;
     });
    
    numLayout = static_cast<int>(layout.size());
    file.close();
    
}

void Stage::AppearLayout()
{
    // overrideする
}

void Stage::UnloadStageData()
{
    // overrideする
}

void Stage::UpdateStage()
{
    // overrideする
}

void Stage::ProcessInput(const struct InputState &state)
{
    StageInput(state);
}

void Stage::StageInput(const struct InputState &state)
{
    
}


