#include "Application.h"
#include "Actor.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"

#include <GL/glew.h>

// コンストラクタ
SpriteComponent::SpriteComponent(Actor* a, int order, bool isBG)
    : Component(a)
    , texture(nullptr)
    , drawOrder(order)
    , texWidth(0)
    , texHeight(0)
    , isBackGround(isBG)
    , isVisible(true)
    , scWidth(1.0f)
    , scHeight(1.0f)
{
    if(isBG)
    {
        owner->GetApp()->GetRenderer()->AddBackGroundSprite(this);
    }
    else
    {
        owner->GetApp()->GetRenderer()->AddSprite(this);
    }
}

// デストラクタ
SpriteComponent::~SpriteComponent()
{
    
    if(isBackGround)
    {
        owner->GetApp()->GetRenderer()->RemoveBackGroundSprite(this);
    }
    else
    {
        owner->GetApp()->GetRenderer()->RemoveSprite(this);
    }
}

// 描画
void SpriteComponent::Draw(Shader* shader)
{
    if (!isVisible)
    {
        return;
    }
    
	if (texture)
    {
		// Textureサイズからスケールマトリックスを作成
        
        float width = static_cast<float>(texWidth);
        float height = static_cast<float>(texHeight);
        
        if (scWidth != 1.0f)
        {
            width = width * scWidth;
        }
        if (scHeight != 1.0f)
        {
            height = height * scHeight;
        }
        
        Matrix4 scaleMat = Matrix4::CreateScale(width, height, 1.0f);
    
	//	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texWidth), static_cast<float>(texHeight), 1.0f);
		// ワールド座標生成
        Matrix4 world = scaleMat * owner->GetWorldTransform();
		
        // シェーダー に送る
		shader->SetMatrixUniform("uWorldTransform", world);
		// アクティブ化
		texture->SetActive();
        // 描画
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

// テクスチャを設定
void SpriteComponent::SetTexture(Texture* tex)
{
	texture = tex;
	texWidth = texture->GetWidth();
	texHeight = texture->GetHeight();
}
