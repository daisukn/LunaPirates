#include "BillboardComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Application.h"
#include "Actor.h"
#include "Renderer.h"

BillboardComponent::BillboardComponent(class Actor* a, int drawOrder)
    : Component(a)
{
    owner->GetApp()->GetRenderer()->AddBillboardComp(this);
}

BillboardComponent::~BillboardComponent()
{
    owner->GetApp()->GetRenderer()->RemoveBillboardComp(this);
}


void BillboardComponent::Draw(Shader* shader)
{
    if(!isVisible) return;
    
    if (texture)
    {
        // Ownerのマトリックスを取得（Positionでも良いかもしれない。）
        Matrix4 mat = owner->GetWorldTransform();
        
        // ビルボーディング
        // ビューマトリックスの逆行列の座標を差し替えて流用
        Matrix4 invVew = owner->GetApp()->GetRenderer()->GetInvViewMatrix();
        // 座標差し替え
        invVew.mat[3][0] = mat.mat[3][0];
        invVew.mat[3][1] = mat.mat[3][1];
        invVew.mat[3][2] = mat.mat[3][2];
        
        // スケールを復元
        Matrix4 scaleMat = Matrix4::CreateScale(texture->GetWidth(), texture->GetHeight(), 1);
        Matrix4 world = scaleMat * invVew;

        // シェーダー に送る
        shader->SetMatrixUniform("uWorldTransform", world);
        texture->SetActive();
        

        shader->SetVectorUniform("uPosition", Vector3(0,0,0));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void BillboardComponent::SetTexture(Texture* tex)
{
    texture = tex;
}

void BillboardComponent::Update(float deltaTime)
{
    
}
