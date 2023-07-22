#ifndef __BILLBOARDCOMPONENT_H
#define __BILLBOARDCOMPONENT_H

#include "Component.h"

// ビルボードコンポーネント
class BillboardComponent : public Component
{
public:
    BillboardComponent(class Actor* a, int drawOrder = 100);
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

private:
    int drawOrder;
    bool isVisible;
    float scale;
    class Texture* texture;

};




#endif // __BILLBOARDCOMPONENT_H
