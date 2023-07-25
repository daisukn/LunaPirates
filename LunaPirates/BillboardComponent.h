#pragma once

#include "Component.h"
#include "Math.h"

// ビルボードコンポーネント
class BillboardComponent : public Component
{
public:
    BillboardComponent(class Actor* a, int order = 100);
    ~BillboardComponent();
    
    // Rendererから呼ばれる
    virtual void Draw(class Shader* shader);

    // テクスチャセット
    virtual void SetTexture(class Texture* tex);
    
    // アップデート処理
    void Update(float deltaTime) override;
    

    void SetVisible(bool visible) { isVisible = visible; }
    bool GetVisible() const { return isVisible; }
    
    void SetScale(float f){ scale = f;}

    int GetDrawOrder() const { return drawOrder; }
    
    
    void SetBlendAdd(bool b) { isBlendAdd = b; }
    bool GetBlendAdd() const { return isBlendAdd; }
    
private:
    int drawOrder;
    bool isVisible;
    float scale;
    class Texture* texture;
    
    // 加算合成するか
    bool isBlendAdd;

};

