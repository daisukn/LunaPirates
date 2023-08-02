#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Application.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"


SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
    : MeshComponent(owner, true)
    , animTime(0.0f)
{
}

void SkeletalMeshComponent::SetAnimID(const unsigned int animID, const PlayMode mode)
{
    mesh->SetAnimID(animID, mode);
}

bool SkeletalMeshComponent::GetIsPlaing() const
{
    return mesh->GetIsPlaying();
}

void SkeletalMeshComponent::Draw(Shader* shader)
{
    if (mesh)
    {
        //glDisable( GL_CULL_FACE );

        // ワールドマトリックスを送る
        Matrix4 m = Matrix4::CreateScale(scale);
        shader->SetMatrixUniform("uWorldTransform", m * owner->GetWorldTransform());

        std::vector<Matrix4> transform;
        mesh->BoneTransform(animTime, transform);

        shader->SetMatrixUniforms("uMatrixPalette", transform.data(), (unsigned int)transform.size());
        shader->SetFloatUniform("uSpecPower", mesh->GetSpecPower());


        // Vertex Arrayを描画
        std::vector<VertexArray*> va = mesh->GetVertexArray();
        for (auto v : va)
        {
            Texture* t = mesh->GetTexture(v->GetTextureID());
            
            if (t)
            {
                t->SetActive();
            }
            
            v->SetActive();
            glDrawElements(GL_TRIANGLES, v->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
//            glDrawElements(GL_TRIANGLES, v->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
        
        if (isToon)
        {
            glFrontFace(GL_CW);
            Matrix4 m = Matrix4::CreateScale(contourFactor * scale);
            shader->SetMatrixUniform("uWorldTransform", m * owner->GetWorldTransform());
            for (auto v : va)
            {
                Texture* t = owner->GetApp()->GetRenderer()->GetTexture("Assets/black.png");
                if (t)
                {
                    t->SetActive();
                }
                v->SetActive();
                glDrawElements(GL_TRIANGLES, v->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
            }
            glFrontFace(GL_CCW);
        }
        
        if (isGlory)
        {
            glBlendFunc(GL_ONE, GL_ONE);
            glFrontFace(GL_CW);
            Matrix4 m = Matrix4::CreateScale(contourFactor*1.05);
            shader->SetMatrixUniform("uWorldTransform", m*owner->GetWorldTransform());
            for (auto v : va)
            {
                Texture* t = owner->GetApp()->GetRenderer()->GetTexture("Assets/glory.png");
                if (t)
                {
                    t->SetActive();
                }
                v->SetActive();
                glDrawElements(GL_TRIANGLES, v->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
            }
            glFrontFace(GL_CCW);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        
    }
}

void SkeletalMeshComponent::Update(float deltaTime)
{
    animTime = deltaTime;
    
}
