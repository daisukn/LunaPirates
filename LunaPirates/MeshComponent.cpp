
#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Application.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

#include <GL/glew.h>
#include <vector>

// コンストラクタ
MeshComponent::MeshComponent(Actor* a, bool isSkeletal, MeshType type)
    : Component(a)
    , mesh(nullptr)
    , textureIndex(0)
    , isVisible(true)
    , isSkeletal(isSkeletal)
    , isToon(false)
    , contourFactor(1.1014f)
    , isBlendAdd(false)
    , isGlory(false)
    , meshType(type)
    , scale(1.0f)
{

    switch(meshType)
    {
    case MESH_EFFECT:
        owner->GetApp()->GetRenderer()->AddEffectMeshComp(this);
        break;
    case MESH_BG:
        owner->GetApp()->GetRenderer()->AddBackGroudMeshComp(this);
        break;
    case MESH_NORMAL:
        owner->GetApp()->GetRenderer()->AddMeshComp(this);
        break;
    }
}

// デストラクタ
MeshComponent::~MeshComponent()
{
    switch(meshType)
    {
    case MESH_EFFECT:
        owner->GetApp()->GetRenderer()->RemoveEffectMeshComp(this);
        break;
    case MESH_BG:
        owner->GetApp()->GetRenderer()->RemoveBackGroudMeshComp(this);
        break;
    case MESH_NORMAL:
        owner->GetApp()->GetRenderer()->RemoveMeshComp(this);
        break;
    }
}

// 描画
void MeshComponent::Draw(Shader* s)
{
	if (mesh)
	{
        if (isBlendAdd)
        {
            glBlendFunc(GL_ONE, GL_ONE);
        }
        
        // WorldマトリックスをShaderに送る
        Matrix4 m = Matrix4::CreateScale(scale);
        s->SetMatrixUniform("uWorldTransform", m * owner->GetWorldTransform());

		// SpecPowerを送る
        s->SetFloatUniform("uSpecPower", mesh->GetSpecPower());


		// Vertex Array
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
        }
        
        // 輪郭強調用
        if (isToon)
        {
            glFrontFace(GL_CW);
            Matrix4 m = Matrix4::CreateScale(contourFactor * scale);
            s->SetMatrixUniform("uWorldTransform", m * owner->GetWorldTransform());
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
            Matrix4 m = Matrix4::CreateScale(contourFactor*scale);
            s->SetMatrixUniform("uWorldTransform", m*owner->GetWorldTransform());
            for (auto v : va)
            {
                Texture* t = mesh->GetTexture(v->GetTextureID());
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
        
        
        if (isBlendAdd)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
	}
}


VertexArray* MeshComponent::GetVertexArray(int id) const
{
    return mesh->GetVertexArray()[id];
}

