#include "Application.h"
#include "Actor.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"

#include <GL/glew.h>

// コンストラクタ
SpriteComponent::SpriteComponent(Actor* actor, int drawOrder)
    : Component(actor)
    , texture(nullptr)
    , drawOrder(drawOrder)
    , texWidth(0)
    , texHeight(0)
{
    owner->GetApp()->GetRenderer()->AddSprite(this);
}

// デストラクタ
SpriteComponent::~SpriteComponent()
{
    // Rendererから削除
    owner->GetApp()->GetRenderer()->RemoveSprite(this);
}

// 描画
void SpriteComponent::Draw(Shader* shader)
{
	if (texture){
		// Textureサイズからスケールマトリックスを作成
		Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texWidth), static_cast<float>(texHeight), 1.0f);
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
