#pragma once

#include "Component.h"
#include "Math.h"

#include <vector>

// パーティクルの破片
struct ParticleParts
{

    bool isVisible;
    Vector3 pos;
    Vector3 dir;
    float size;
    float lifeTime;

    
    ParticleParts()
        :pos(Vector3(0.0f, 0.0f, 0.0f))
        ,dir(Vector3(0.0f, 0.0f, 0.0f))
        ,isVisible(false)
        ,lifeTime(0.0f)
        ,size(0.0)

    {}
};


// パーティクルコンポーネント（SpriteComponentから継承した方が良いかもしれない）
class ParticleComponent : public Component
{
public:
    enum ParticleMode
    {
        P_SPARK,
        P_SMOKE,
        P_WATER
    };
    
    
    ParticleComponent(class Actor* a, int drawOrder = 100);
    ~ParticleComponent();
    
    // Rendererから呼ばれる
    virtual void Draw(class Shader* shader);

    // テクスチャセット
    virtual void SetTexture(class Texture* tex);
    
    // アップデート処理
    void Update(float deltaTime) override;

    // パーティクル発生
    void CreateParticles(Vector3 pos, unsigned int num, float life, float part_life, float size, ParticleMode mode = P_SPARK);
    
    
    bool GetVisible() const { return isVisible; }
    int GetDrawOrder() const { return drawOrder; }
    
    void SetParticleSpeed(float f) { partSpeed = f; }
    float GetParticleSpeed() const { return partSpeed; }

    
    void SetBlendAdd(bool b) { isBlendAdd = b; }
    bool GetBlendAdd() const { return isBlendAdd; }
private:
    // 所有アクターとの相対位置
    Vector3 position;
    bool isVisible;
    unsigned int numParts;
    float lifeTime;
    float totalLife;
    float partLifecycle;
    float partSize;

    float partSpeed;

    ParticleMode partMode;
    //bool isGravity;
    int drawOrder;
    // パーティクル破片を生成
    void GenerateParts();

    std::vector<ParticleParts> parts;
    
    class Texture* texture;
    
    // 加算合成するか
    bool isBlendAdd;
    

}; 


