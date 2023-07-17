#pragma once

#include "Component.h"

// スプライト管理 Componentを継承
class SpriteComponent : public Component
{
public:
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* texture);

    int GetDrawOrder() const { return drawOrder; }
    int GetTexHeight() const { return texHeight; }
    int GetTexWidth() const { return texWidth; }

    
protected:
    // スプライト用のテクスチャ
    class Texture* texture;
    int drawOrder;
    int texWidth;
    int texHeight;


};


