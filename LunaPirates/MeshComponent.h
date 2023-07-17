#pragma once

#include "Component.h"
//#include <cstddef>

// Meshを管理するComponent（Rendererから呼ばれる）
class MeshComponent : public Component
{
public:
    MeshComponent(class Actor* owner,  bool isSkeletal = false, bool isBG = false);
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
    
    
protected:
    class Mesh* mesh;      // メッシュ
    unsigned int textureIndex;    // TextureID
    
    bool isVisible;
    bool isSkeletal;
    
    // 輪郭強調
    bool isToon;
    float contourFactor;
    
    // 加算合成するか
    bool isBlendAdd;
};

