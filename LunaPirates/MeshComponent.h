#pragma once

#include "Component.h"
#include "Animation.h"
//#include <cstddef>

enum MeshType
{
    MESH_NORMAL,
    MESH_BG,
    MESH_EFFECT
};

// Meshを管理するComponent（Rendererから呼ばれる）
class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* a, bool isSkeletal = false, MeshType type = MESH_NORMAL);
    virtual ~MeshComponent();
        
    // 描画 override
    virtual void Draw(class Shader* s);
    virtual void SetMesh(class Mesh* m) { mesh = m; }              // メッシュセット
    void SetTextureIndex(unsigned int index) { textureIndex = index; }    // テクスチャGetter

    void SetVisible(bool visible) { isVisible = visible; }
    bool GetVisible() const { return isVisible; }
    
    bool GetIsSkeletal() const { return isSkeletal; }
    class VertexArray* GetVertexArray(int id) const;
    
    void SetToonRender(const bool t, const float f = 1.05f) { isToon = t; contourFactor = f; }
    void SetContourFactor(const float f) { contourFactor = f; }
    
    bool GetToon() const { return isToon; }
    
    void SetBlendAdd(bool b) { isBlendAdd = b; }
    bool GetBlendAdd() const { return isBlendAdd; }
    
    void SetGlory(bool b) { isGlory = b; }
    bool GetGlory() const { return isGlory; }
    
    void SetScale(float f) { scale = f; }
    float GetScale() const { return scale; }
    
    // 再生するモーションのID（SkerltalMeshでオーバーライドする。インターフェース確保のため）
    virtual void SetAnimID(const unsigned int animID, const PlayMode mode){}
    
protected:
    class Mesh* mesh;      // メッシュ
    unsigned int textureIndex;    // TextureID
    
    float scale;
    
    bool isVisible;
    bool isSkeletal;
    
    MeshType meshType;
    
    
    // 輪郭強調
    bool isToon;
    float contourFactor;
    
    // 後光
    bool isGlory;
    
    // 加算合成するか
    bool isBlendAdd;
};

