#pragma once

#include "Component.h"
#include "DebuggerComponent.h"

#include "Math.h"

#include <vector>
#include <memory>


// OBB計算用のStruct
struct OBB
{
    Vector3 pos;    // 中心座標
    Vector3 radius; // 半径
    Vector3 rot;    // 回転角度
    Vector3 axisX;  // 分離軸X
    Vector3 axisY;  // 分離軸Y
    Vector3 axisZ;  // 分離軸Z

    Vector3 min;
    Vector3 max;
    
    void operator = (const OBB& src)
    {
        OBB::min = src.min;
        OBB::max = src.max;
        
        OBB::pos = src.pos;
        OBB::radius = src.radius;
        OBB::rot = src.rot;
        OBB::axisX = src.axisX;
        OBB::axisY = src.axisY;
        OBB::axisZ = src.axisZ;
    }


};


// バウンディングボリューム管理
class BoundingVolumeComponent : //public Component,
                                public DebuggerComponent
{
public:
    // コンストラクタ
    BoundingVolumeComponent(class Actor* a);
    // デストラクタ
    ~BoundingVolumeComponent();
    // VertexArrayからBaoundingボリューム生成
    void ComputeBoundingVolume(const std::vector<class VertexArray*> va);
    // Min Maxを直接設定
    void ComputeBoundingVolume(const Vector3 min, const Vector3 max);
    
    // バウンディングボックスを手動調整するヘルパー
    void AdjustBoundingBox(const Vector3 pos, const Vector3 sc);
    // デバッグ用
    void CreateVArray();
    
    // 座標系のUpdate
    void OnUpdateWorldTransform() override;
    
    // 描画 override
    void Draw(class Shader* shader) override;
    
    // バウンディングボックス取得
    struct Cube* GetBoundingBox() const { return boundingBox.get(); }
    struct OBB* GetOBB() const { return obb.get(); }
    
    struct Polygon* GetPlygons() const { return polygons.get(); }
    
    void SetVisible(bool b) { isVisible = b; }
    bool GetVisibile() const { return isVisible; }
    
    float GetRadius() const { return radius; }
    void SetRadius(float f) { radius = f; }

private:
 
    // デバッグ用に表示するかどうか
    bool isVisible;
    
    std::unique_ptr<struct Polygon[]> polygons;
    void CreatePolygons();

    std::unique_ptr<class VertexArray> vArray;
    //class Texture* texture;
    

    std::unique_ptr<struct Cube> boundingBox;
    std::unique_ptr<struct OBB> obb;
    float radius;
    
};

