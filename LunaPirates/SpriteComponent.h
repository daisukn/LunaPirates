#pragma once

#include "Component.h"

// スプライト管理 Componentを継承
class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* a, int order = 100, bool isBG = false);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* texture);

    int GetDrawOrder() const { return drawOrder; }
    int GetTexHeight() const { return texHeight; }
    int GetTexWidth() const { return texWidth; }

    void SetVisible(bool visible) { isVisible = visible; }
    bool GetVisible() const { return isVisible; }
    
    void SetScale(float w, float h) { scWidth = w; scHeight = h; }
    
protected:
    // スプライト用のテクスチャ
    class Texture* texture;
    int drawOrder;
    int texWidth;
    int texHeight;
    
    float scWidth;
    float scHeight;
    
    bool isBackGround;
    bool isVisible;


};


