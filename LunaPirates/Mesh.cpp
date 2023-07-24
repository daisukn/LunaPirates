#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Bone.h"
#include "Polygon.h"


// Assimp用
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#ifndef ASSIMP_LOAD_FLAGS
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate  | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_MakeLeftHanded)
#endif //ASSIMP_LOAD_FLAGS


#include <vector>
#include <memory>
#include <iostream>
#include <cassert>


// aiMatrix4x4からMatrix4へ変換
static void MatrixAi2Gl(Matrix4& mat, const aiMatrix4x4 aim)
{
    mat.mat[0][0] = aim.a1;
    mat.mat[0][1] = aim.b1;
    mat.mat[0][2] = aim.c1;
    mat.mat[0][3] = aim.d1;

    mat.mat[1][0] = aim.a2;
    mat.mat[1][1] = aim.b2;
    mat.mat[1][2] = aim.c2;
    mat.mat[1][3] = aim.d2;

    mat.mat[2][0] = aim.a3;
    mat.mat[2][1] = aim.b3;
    mat.mat[2][2] = aim.c3;
    mat.mat[2][3] = aim.d3;

    mat.mat[3][0] = aim.a4;
    mat.mat[3][1] = aim.b4;
    mat.mat[3][2] = aim.c4;
    mat.mat[3][3] = aim.d4;    

}


// コンストラクタ
Mesh::Mesh()
    : pScene(nullptr)
    , numBones(0)
    , specPower(1.1f)
    , animID(0)
    , prevAnimID(0)
    , numAnimations(0)
    , playTime(0.0f)
    , mode(PLAY_ONCE)
    , isPlaying(false)
{
    
}

Mesh::~Mesh()
{
    while (!vArray.empty())
    {
        delete vArray.back();
        vArray.pop_back();
    }
}


void Mesh::SetAnimID(int id, PlayMode m)
{
    if(prevAnimID != id)
    {
        playTime = 0.0f;
        isPlaying = true;
    }

    prevAnimID = animID;
    animID = id;
    mode = m;
}

// アニメーション時間の反映
void Mesh::BoneTransform(float deltaTime, std::vector<Matrix4>& transforms)
{
    
    playTime += deltaTime;
    float ticksPerSecond = (float)(pScene->mAnimations[animID]->mTicksPerSecond != 0 ? pScene->mAnimations[animID]->mTicksPerSecond : 25.0f);
    float timeInTicks = playTime * ticksPerSecond;


    if (mode == PLAY_ONCE)
    {
        if (timeInTicks > durations[animID])
        {
            // 再生が終わっている場合は、最後のポーズに固定
            playTime = durations[animID];
            isPlaying = false;
        }
    }
    
    Matrix4 identity = Matrix4::Identity;
    

    
    float animationTime = fmod(timeInTicks, (float)pScene->mAnimations[animID]->mDuration);
    
    ReadNodeHeirarchy(animationTime, pScene->mRootNode, identity);

    transforms.resize(numBones);

    for (unsigned int i = 0; i < numBones; i++)
    {
         transforms[i] = boneInfo[i].FinalTransformation;
    }
}

// 階層を辿ってノードの変換行列を得る
void Mesh::ReadNodeHeirarchy(float animationTime, const aiNode* pNode, const Matrix4& parentTransform)
{
    std::string nodeName(pNode->mName.data);

    
    const aiAnimation* pAnimation = pScene->mAnimations[animID];
    
        
    Matrix4 nodeTransformation;
    MatrixAi2Gl(nodeTransformation, pNode->mTransformation);
     
    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, nodeName);
    
    if (pNodeAnim)
    {
        // スケーリング
        Vector3 scaling;
        CalcInterpolatedScaling(scaling, animationTime, pNodeAnim);
        Matrix4 scalingM = Matrix4::CreateScale(scaling);

        // 回転マトリックス
        Quaternion rotationQ;
        CalcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
        Matrix4 rotationM = Matrix4::CreateFromQuaternion(rotationQ);
    
        // 移動マトリックス
        Vector3 translation;
        CalcInterpolatedPosition(translation, animationTime, pNodeAnim);
        Matrix4 translationM = Matrix4::CreateTranslation(translation);

        // ローカルマトリックスを生成
        nodeTransformation =  rotationM * translationM * scalingM;

    }

    
    
    // グローバルマトリックスを生成
    Matrix4 globalTransformation = nodeTransformation * parentTransform * globalInverseTransform;
    
    if (boneMapping.find(nodeName) != boneMapping.end())
    {
        unsigned int boneIndex = boneMapping[nodeName];
        boneInfo[boneIndex].FinalTransformation =  boneInfo[boneIndex].BoneOffset * globalTransformation;
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; i++)
    {
        ReadNodeHeirarchy(animationTime, pNode->mChildren[i], globalTransformation);
    }
}

// アニメーションデータを取得
const aiNodeAnim* Mesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string nodeName)
{
    for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++)
    {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        
        if (std::string(pNodeAnim->mNodeName.data) == nodeName)
        {
            return pNodeAnim;
        }
    }
    return nullptr;
}


void Mesh::CalcInterpolatedPosition(Vector3& outVec, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1)
    {
        outVec.Set(pNodeAnim->mPositionKeys[0].mValue.x,
                pNodeAnim->mPositionKeys[0].mValue.y,
                pNodeAnim->mPositionKeys[0].mValue.z);
        return;
    }
            
    unsigned int positionIndex = FindPosition(animationTime, pNodeAnim);
    unsigned int nextPositionIndex = (positionIndex + 1);
    assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);
    
    float deltaTime = (float)(pNodeAnim->mPositionKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);
    float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;
    aiVector3D tr = start + factor * delta;
    outVec.Set(tr.x, tr.y, tr.z);
}


void Mesh::CalcInterpolatedRotation(Quaternion& outVec, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys == 1)
    {
        outVec.Set(pNodeAnim->mRotationKeys[0].mValue.x,
                pNodeAnim->mRotationKeys[0].mValue.y,
                pNodeAnim->mRotationKeys[0].mValue.z,
                pNodeAnim->mRotationKeys[0].mValue.w);
        return;
    }
    
    unsigned int rotationIndex = FindRotation(animationTime, pNodeAnim);
    unsigned int nextRotationIndex = (rotationIndex + 1);
    assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);
    
    float deltaTime = (float)(pNodeAnim->mRotationKeys[nextRotationIndex].mTime - pNodeAnim->mRotationKeys[rotationIndex].mTime);
    float factor = (animationTime - (float)pNodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion& startRotationQ = pNodeAnim->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion& endRotationQ   = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;

    
    
    aiQuaternion q;
    aiQuaternion::Interpolate(q, startRotationQ, endRotationQ, factor);
    q = q.Normalize();
    outVec.Set(q.x, q.y, q.z, q.w);

}


void Mesh::CalcInterpolatedScaling(Vector3& outVec, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1)
    {
        outVec.Set(pNodeAnim->mScalingKeys[0].mValue.x,
                pNodeAnim->mScalingKeys[0].mValue.y,
                pNodeAnim->mScalingKeys[0].mValue.z);
        return;
    }

    unsigned int scalingIndex = FindScaling(animationTime, pNodeAnim);
    unsigned int nextScalingIndex = (scalingIndex + 1);
    
    assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);
    float deltaTime = (float)(pNodeAnim->mScalingKeys[nextScalingIndex].mTime - pNodeAnim->mScalingKeys[scalingIndex].mTime);
    float factor = (animationTime - (float)pNodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);
    
    const aiVector3D& start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
    const aiVector3D& end   = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
    
    aiVector3D delta = end - start;
    aiVector3D sc = start + factor * delta;
    outVec.Set(sc.x, sc.y, sc.z);
}


unsigned int Mesh::FindPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
    {
        if (animationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


unsigned int Mesh::FindRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
    {
        if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


unsigned int Mesh::FindScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
    {
        if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
        {
            return i;
        }
    }
    
    assert(0);

    return 0;
}


// アニメーションデータ読み込み
void Mesh::LoadAnimation()
{
    numAnimations = pScene->mNumAnimations;
    durations.resize(numAnimations);
    for (int i = 0; i < numAnimations; i++)
    {
        durations[i] = (float)pScene->mAnimations[i]->mDuration;
        
    }
    
}

// ボーン読み込み
void Mesh::LoadBones(const aiMesh* m, std::vector<VertexBoneData>& bones)
{
    for (unsigned int i = 0 ; i < m->mNumBones ; i++)
    {
        unsigned int boneIndex = 0;
        std::string boneName(m->mBones[i]->mName.data);
        
        if (boneMapping.find(boneName) == boneMapping.end())
        {
            // Allocate an index for a new bone
            boneIndex = numBones;
            numBones++;
            BoneInfo bi;
            boneInfo.push_back(bi);
            
            // オフセットマトリックスのコピー
            Matrix4 mat;
            MatrixAi2Gl(mat, m->mBones[i]->mOffsetMatrix);
            boneInfo[boneIndex].BoneOffset = mat;
            boneMapping[boneName] = boneIndex;
        }
        else
        {
            boneIndex = boneMapping[boneName];
        }
        
        for (unsigned int j = 0 ; j < m->mBones[i]->mNumWeights ; j++)
        {
            unsigned int VertexID = m->mBones[i]->mWeights[j].mVertexId;

            float weight  = m->mBones[i]->mWeights[j].mWeight;
            bones[VertexID].AddBoneData(boneIndex, weight);
        }
    }
}

// Bone付き
void Mesh::CreateMeshBone(const aiMesh* m)
{

    std::vector<float> vertexBuffer;            // ３個ずつ（XYZ）
    std::vector<float> normalBuffer;            // ３個ずつ
    std::vector<float> uvBuffer;                // ２個ずつ
    std::vector<unsigned int> boneIDs;          // ４個ずつ
    std::vector<float> boneWeights;             // ４個ずつ
    std::vector<unsigned int> indexBuffer;
    
    std::vector<VertexBoneData> bones;
    // Boneバッファ
    bones.resize(m->mNumVertices);

    LoadBones(m, bones);
    
    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        vertexBuffer.push_back(m->mVertices[i].x);
        vertexBuffer.push_back(m->mVertices[i].y);
        vertexBuffer.push_back(m->mVertices[i].z);
            
        normalBuffer.push_back(-m->mNormals[i].x);
        normalBuffer.push_back(m->mNormals[i].y);
        normalBuffer.push_back(m->mNormals[i].z);

        if (m->HasTextureCoords(0))
        {
            uvBuffer.push_back(m->mTextureCoords[0][i].x);
            uvBuffer.push_back(m->mTextureCoords[0][i].y);
        }
        else
        {
            uvBuffer.push_back(0.0f);
            uvBuffer.push_back(0.0f);
        }

        boneIDs.push_back(bones[i].IDs[0]);
        boneIDs.push_back(bones[i].IDs[1]);
        boneIDs.push_back(bones[i].IDs[2]);
        boneIDs.push_back(bones[i].IDs[3]);
        
        boneWeights.push_back(bones[i].Weights[0]);
        boneWeights.push_back(bones[i].Weights[1]);
        boneWeights.push_back(bones[i].Weights[2]);
        boneWeights.push_back(bones[i].Weights[3]);
    }

    // インデックスバッファ取り出し
    for (unsigned int i = 0; i < m->mNumFaces; i++)
    {
        const aiFace& Face = m->mFaces[i];
        assert(Face.mNumIndices == 3);
        indexBuffer.push_back((Face.mIndices[0]));
        indexBuffer.push_back((Face.mIndices[1]));
        indexBuffer.push_back((Face.mIndices[2]));
    }
    
    
    VertexArray* va = new VertexArray(static_cast<unsigned int>(vertexBuffer.size()) / 3,
                                  vertexBuffer.data(),
                                  normalBuffer.data(),
                                  uvBuffer.data(),
                                  boneIDs.data(),
                                  boneWeights.data(),
                                  static_cast<unsigned int>(indexBuffer.size()),
                                  indexBuffer.data());
    
    va->SetTextureID(m->mMaterialIndex);
    
    vArray.push_back(va);
    
    LoadAnimation();
    
    

}

// メッシュをロード（Boneなし）
void Mesh::CreateMesh(const aiMesh* m)
{
    
    std::vector<float> vertexBuffer;            // ３個ずつ（XYZ）
    std::vector<float> normalBuffer;            // ３個ずつ
    std::vector<float> uvBuffer;                // ２個ずつ
    std::vector<unsigned int> indexBuffer;

    for (unsigned int i = 0 ; i < m->mNumVertices ; i++)
    {
        vertexBuffer.push_back(m->mVertices[i].x);
        vertexBuffer.push_back(m->mVertices[i].y);
        vertexBuffer.push_back(m->mVertices[i].z);
        
        normalBuffer.push_back(m->mNormals[i].x);
        normalBuffer.push_back(m->mNormals[i].y);
        normalBuffer.push_back(m->mNormals[i].z);

        if (m->HasTextureCoords(0))
        {
            uvBuffer.push_back(m->mTextureCoords[0][i].x);
            uvBuffer.push_back(m->mTextureCoords[0][i].y);
        }
        else
        {
            uvBuffer.push_back(0.0f);
            uvBuffer.push_back(0.0f);
        }
    }
    
    // インデックスバッファ取り出し
    for (unsigned int i = 0 ; i < m->mNumFaces ; i++)
    {

        const aiFace& Face = m->mFaces[i];
        assert(Face.mNumIndices == 3);
        indexBuffer.push_back(Face.mIndices[0]);
        indexBuffer.push_back(Face.mIndices[1]);
        indexBuffer.push_back(Face.mIndices[2]);
    }

    VertexArray* va = new VertexArray(static_cast<unsigned int>(vertexBuffer.size()) / 3,
                                      vertexBuffer.data(),
                                      normalBuffer.data(),
                                      uvBuffer.data(),
                                      static_cast<unsigned int>(indexBuffer.size()),
                                      indexBuffer.data());
    
    va->SetTextureID(m->mMaterialIndex);
    vArray.push_back(va);
    
}

// Assimpを使ったモデルデータロード
bool Mesh::Load(const std::string& fileName, class Renderer* r)
{
    
    // 読み込み（全データはaiSceneに格納される）
    pScene = importer.ReadFile(fileName.c_str(), ASSIMP_LOAD_FLAGS);
    if(!pScene)
    {
        return false;
    }


    MatrixAi2Gl(globalInverseTransform, pScene->mRootNode->mTransformation);
//    GlobalInverseTransform.Invert();
    

    // 全メッシュ読み込み
    for (int cnt = 0; cnt < pScene->mNumMeshes; cnt++)
    {
        aiMesh* m = pScene->mMeshes[cnt];
        
        // ボーン情報有無
        if (m->HasBones())
        {
            CreateMeshBone(m);
            
        }
        else
        {
            CreateMesh(m);
        }
        
        
    }
    

    // テクスチャ読み込み
    for (unsigned int cnt = 0; cnt < pScene->mNumMaterials; cnt++){
    
        aiString Path;
        aiMaterial* pMaterial = pScene->mMaterials[cnt];
        Texture* t;
        if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
        {
            std::string texturefilename = Path.data;
            // テクスチャ読み込み
            t = r->GetTexture("Assets/Textures/" + texturefilename);
        }
        else
        {
            t = r->GetTexture("Assets/Textures/Default.png");
        }
        textures.push_back(t);
    }
    
    
    return true;
}


// データ破棄
void Mesh::Unload()
{
    pScene = nullptr;
    vArray.clear();
}

// テクスチャIDからGetter
Texture* Mesh::GetTexture(size_t index)
{
    if (index < textures.size())
    {
        return textures[index];
    }
    else
    {
        return nullptr;
    }
}





