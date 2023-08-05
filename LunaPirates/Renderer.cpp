#include "Renderer.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "ParticleComponent.h"
#include "BillboardComponent.h"

#include "DebuggerComponent.h"

#include <GL/glew.h>
#include <algorithm>


//#define __GAME_DEBUG
//#define __FULLSCREEN__

// コンストラクタ
Renderer::Renderer()
    : strTitle("")
    , screenWidth(0.0f)
    , screenHeight(0.0f)
    , window(nullptr)
    , glContext(nullptr)
{
}

// デストラクタ
Renderer::~Renderer()
{
}


// ウィンドウ生成とGL初期化
bool Renderer::Initialize(std::string title, float scWidth, float scHeight)
{
    strTitle        = title;    // ウィンドウタイトル
    screenWidth     = scWidth;  // スクリーン幅
    screenHeight    = scHeight; // スクリーン高さ
    
    
    // OpenGL プロファイル, バージョン
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // ダブルバッファリング
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // RGBAチャンネル
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // ハードウェアアクセラレーション有効化
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    

#ifdef __FULLSCREEN__
    unsigned int WINDOW_FLAGS = SDL_WINDOW_OPENGL || SDL_WINDOW_FULLSCREEN;
#else
    unsigned int WINDOW_FLAGS = SDL_WINDOW_OPENGL;
#endif // __FULLSCREEN__

    
    //ウインドウ生成
    window = SDL_CreateWindow(strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(screenWidth), static_cast<int>(screenHeight), WINDOW_FLAGS);
    
    if (!window )
    {
        std::cout << "Unable to create window" << std::endl;
        return false;
    }
    
    
    // OpenGL コンテキスト生成
    glContext = SDL_GL_CreateContext(window);
    
    // GLEW初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }
   
    
    /*シェーダー のロードなどはここでやる*/
    LoadShaders();
    CreateSpriteVerts();
 
    //
    SetClearColor(0.596f, 0.733f, 0.858f);
    SetClearColor(0.5f, 0.0f, 0.0f);

    return true;
}

// リリース処理
void Renderer::Shutdown()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

}

void::Renderer::SetClearColor(float r,float g, float b)
{
    glClearColor(r, g, b, 1.0f);
}

// 描画処理
void Renderer::Draw()
{
//    glClearColor(0.596f, 0.733f, 0.858f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Culling設定
    glEnable( GL_CULL_FACE );
    glFrontFace(GL_CCW);

    
    /* 描画処理 メッシュ、スプライト*/
    // アルファブレンディングを有効化
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    
    // 背景スプライト
    glDisable(GL_DEPTH_TEST);
    spriteShader->SetActive();
    for (auto sprite : bgSpriteComps)
    {
        sprite->Draw(spriteShader.get());
    }
    
    
    
    // 背景用メッシュ描画
    glEnable(GL_DEPTH_TEST);
    backGroundShader->SetActive();
    backGroundShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    // Update lighting uniforms
    SetLightUniforms(backGroundShader.get());
    for (auto bg : bgMesh)
    {
        if (bg->GetVisible())
        {
            bg->Draw(backGroundShader.get());
        }
    }
    
    
    
    // メッシュ描画
    meshShader->SetActive();
    for (auto mc : meshComps)
    {
        if (mc->GetVisible())
        {
            if(mc->GetToon())
            {
                meshShaderToon->SetActive();
                SetLightUniforms(meshShader.get());
                meshShaderToon->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
                mc->Draw(meshShader.get());
            }
            else
            {
                meshShader->SetActive();
                SetLightUniforms(meshShaderToon.get());
                meshShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
                mc->Draw(meshShaderToon.get());
            }
        }
    }

    // スキンメッシュ描画
    skinnedShaderToon->SetActive();
    skinnedShaderToon->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    // Update lighting uniforms
    SetLightUniforms(skinnedShaderToon.get());
    for (auto sk : skeletalMeshes)
    {
        if (sk->GetVisible())
        {
            sk->Draw(skinnedShaderToon.get());
        }
    }

#ifdef __GAME_DEBUG
    // デバッガー用の描画
    solidShader->SetActive();
    solidShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    solidShader->SetVectorUniform("uSolColor", Vector3(0.2f, 0.2f, 1.0f));
    // Update lighting uniforms
    SetLightUniforms(solidShader.get());
    for (auto dg : dbgComps)
    {
        dg->Draw(solidShader.get());

    }
#endif // __GAME_DEBUG
    
    
    
    // パーティクルの処理
    // Zバッファに書き込まない
    glDepthMask(GL_FALSE);
    //加算合成
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_ONE, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spriteVerts->SetActive();
    particleShader->SetActive();
    particleShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);

    for (auto parts : particleComps)
    {
        parts->Draw(particleShader.get());
    }
    //glDepthMask(GL_TRUE);
    
    
    
    
    // ビルボード
    // Zバッファに書き込まない
    glDepthMask(GL_FALSE);

    spriteVerts->SetActive();
    billboardShader->SetActive();
    SetLightUniforms(billboardShader.get());
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    billboardShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);

    for (auto bb : billboardComps)
    {
        
        bb->Draw(billboardShader.get());
    }
    glDepthMask(GL_TRUE);
    
    
    // エフェクトメッシュ描画
    meshShader->SetActive();
    meshShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    // Update lighting uniforms
    SetLightUniforms(meshShader.get());
    for (auto mesh : effectMesh)
    {
        if (mesh->GetVisible())
        {
            mesh->Draw(meshShader.get());
        }
    }
    
    
    // スプライト処理
    glDisable(GL_DEPTH_TEST);
    // アルファブレンディング
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spriteShader->SetActive();    
    for (auto sprite : spriteComps)
    {
        sprite->Draw(spriteShader.get());
    }
    

    SDL_GL_SwapWindow(window);

}

// シェーダー 読み込み
bool Renderer::LoadShaders()
{

    // スプライト用シェーダー生成
    spriteShader = std::make_unique<Shader>();
    if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
    {
        return false;
    }
    spriteShader->SetActive();
    
    // ビューマトリックス、プロジェクションマトリックス
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(screenWidth, screenHeight);
    spriteShader->SetMatrixUniform("uViewProj", viewProj);

    
    
    // Billboard用シェーダー
    billboardShader = std::make_unique<Shader>();
    if (!billboardShader->Load("Shaders/Billboard.vert", "Shaders/Billboard.frag"))
    {
        return false;
    }
    billboardShader->SetActive();
    
    
    // Particle用シェーダー
    particleShader = std::make_unique<Shader>();
    if (!particleShader->Load("Shaders/Billboard.vert", "Shaders/Particle.frag"))
    {
        return false;
    }
    particleShader->SetActive();
    
    // メッシュ用シェーダー生成
    meshShader = std::make_unique<Shader>();
    if (!meshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    meshShader->SetActive();
    
    // メッシュ用シェーダー(Toon)生成
    meshShaderToon = std::make_unique<Shader>();
    if (!meshShaderToon->Load("Shaders/Phong.vert", "Shaders/Toon.frag"))
    {
        return false;
    }
    meshShaderToon->SetActive();
    
    
    
    // 背景用シェーダー生成
    backGroundShader = std::make_unique<Shader>();
    if (!backGroundShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
    {
        return false;
    }
    backGroundShader->SetActive();
    
    
    
    // スキンメッシュ用シェーダー
    skinnedShader = std::make_unique<Shader>();
    if (!skinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    skinnedShader->SetActive();
    
    // スキンメッシュ用シェーダー(Toon)
    skinnedShaderToon = std::make_unique<Shader>();
    if (!skinnedShaderToon->Load("Shaders/Skinned.vert", "Shaders/Toon.frag"))
    {
        return false;
    }
    skinnedShaderToon->SetActive();
    
    
    // ワイヤフレーム用　単色シェーダー生成
    solidShader = std::make_unique<Shader>();
    if (!solidShader->Load("Shaders/BasicMesh.vert", "Shaders/SolidColor.frag"))
    {
        return false;
    }
    solidShader->SetActive();
    

    
    // ビューマトリックス、プロジェクションマトリックス（デフォルト値）
    viewMatrix = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    projectionMatrix = Matrix4::CreatePerspectiveFOV(Math::ToRadians(30.0f), screenWidth, screenHeight, 1.0f, 2000.0f);
    
    // シェーダーに送る
    //meshShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    //skinnedShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    //billboardShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    
    return true;

}

// ライト設定
void Renderer::SetLightUniforms(Shader* shader)
{
    // カメラポジション
    Matrix4 invView = viewMatrix;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // Ambient light
    shader->SetVectorUniform("uAmbientLight", ambientLight);
    // Directional light
    shader->SetVectorUniform("uDirLight.mDirection", dirLight.Direction);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", dirLight.DiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", dirLight.SpecColor);
    
    
    // フォグ
    shader->SetFloatUniform("uFoginfo.maxDist", 800.0f);
    shader->SetFloatUniform("uFoginfo.minDist", 400.0f);
    
//    shader->SetVectorUniform("uFoginfo.color", Vector3(0.75f, 0.96f, 0.99f) );
//    shader->SetVectorUniform("uFoginfo.color", Vector3(0.69f, 0.859f, 0.894f) );
    shader->SetVectorUniform("uFoginfo.color", Vector3(0.69f, 0.859f, 0.894f) );


}

// スプライトコンポーネントの登録
void Renderer::AddSprite(SpriteComponent* sprite)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int drawOrder = sprite->GetDrawOrder();
    auto iter = spriteComps.begin();
    for (;iter != spriteComps.end(); ++iter)
    {
        if (drawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    spriteComps.insert(iter, sprite);
}

// スプライト削除
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = std::find(spriteComps.begin(), spriteComps.end(), sprite);
    if (iter != spriteComps.end())
    { // 要素が見つかった場合のみ削除
        spriteComps.erase(iter);
    }

}

// 背景スプライトコンポーネントの登録
void Renderer::AddBackGroundSprite(SpriteComponent* sprite)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int drawOrder = sprite->GetDrawOrder();
    auto iter = bgSpriteComps.begin();
    for (;iter != bgSpriteComps.end(); ++iter)
    {
        if (drawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    bgSpriteComps.insert(iter, sprite);
}

// スプライト削除
void Renderer::RemoveBackGroundSprite(SpriteComponent* sprite)
{
    auto iter = std::find(bgSpriteComps.begin(), bgSpriteComps.end(), sprite);
    if (iter != bgSpriteComps.end())
    { // 要素が見つかった場合のみ削除
        bgSpriteComps.erase(iter);
    }

}



// テクスチャ取り出し
Texture* Renderer::GetTexture(const std::string &fileName){
 
    Texture* tex = nullptr;
    auto iter = textures.find(fileName);
    if (iter != textures.end())
    {
        tex = iter->second.get();
    }
    else
    {
        std::unique_ptr<Texture> t = std::make_unique<Texture>();
        if(t->Load(fileName))
        {
            tex = t.get();
            textures.emplace(fileName, std::move(t));
        }
        else
        {
            tex = nullptr;
        }
    }
    return tex;
}

//スプライト用ポリゴン
void Renderer::CreateSpriteVerts()
{
    const float vertices[] = {
        -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
        0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
        0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
        -0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
    };
    

    const unsigned int indices[] = {
        2, 1, 0,
        0, 3, 2
    };
    spriteVerts = std::make_unique<VertexArray>((float*)vertices, (unsigned int)4, (unsigned int*)indices, (unsigned int)6);
}



// メッシュ取り出し
Mesh* Renderer::GetMesh(const std::string& fileName)
{
    Mesh* m = nullptr;
    auto iter = meshes.find(fileName);
    if (iter != meshes.end())
    {
        m = iter->second.get();
    }
    else
    {
        std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
        
        if (mesh->Load(fileName, this))
        {
            m = mesh.get();
            meshes.emplace(fileName, std::move(mesh));
        }
        else
        {
            m = nullptr;
        }
    }
    return m;
}

// メッシュコンポーネント登録
void Renderer::AddMeshComp(MeshComponent* mesh)
{
    if (mesh->GetIsSkeletal())
    {
        SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
        skeletalMeshes.emplace_back(sk);
    }
    else
    {

        meshComps.emplace_back(mesh);
    }
 }


// メッシュコンポーネント削除
void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
    if (mesh->GetIsSkeletal())
    {
        SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
        auto iter = std::find(skeletalMeshes.begin(), skeletalMeshes.end(), sk);
        if (iter != skeletalMeshes.end())
        { // 要素が見つかった場合のみ削除
            skeletalMeshes.erase(iter);
        }
    }
    else
    {
        auto iter = std::find(meshComps.begin(), meshComps.end(), mesh);
        if (iter != meshComps.end())
        { // 要素が見つかった場合のみ削除
            meshComps.erase(iter);
        }
    }
}

// BGメッシュコンポーネント登録
void Renderer::AddBackGroudMeshComp(MeshComponent* mesh)
{
    bgMesh.emplace_back(mesh);
}


// BGメッシュコンポーネント削除
void Renderer::RemoveBackGroudMeshComp(MeshComponent* mesh)
{
    auto iter = std::find(bgMesh.begin(), bgMesh.end(), mesh);
    if (iter != bgMesh.end())
    { // 要素が見つかった場合のみ削除
        bgMesh.erase(iter);
    }
 
}

// エフェクトメッシュコンポーネント登録
void Renderer::AddEffectMeshComp(MeshComponent* mesh)
{
    effectMesh.emplace_back(mesh);
}


// BGメッシュコンポーネント削除
void Renderer::RemoveEffectMeshComp(MeshComponent* mesh)
{
    auto iter = std::find(effectMesh.begin(), effectMesh.end(), mesh);
    if (iter != effectMesh.end())
    { // 要素が見つかった場合のみ削除
        effectMesh.erase(iter);
    }
 
}


// パーティクルコンポーネント登録
void Renderer::AddParticleComp(ParticleComponent* part)
{
    int myDrawOrder = part->GetDrawOrder();
    auto iter = particleComps.begin();
    for (;iter != particleComps.end(); ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }
    particleComps.insert(iter, part);
}

// パーティクルコンポーネント削除
void Renderer::RemoveParticleComp(ParticleComponent* part)
{
    auto iter = std::find(particleComps.begin(), particleComps.end(), part);
    if (iter != particleComps.end())
    { // 要素が見つかった場合のみ削除
        particleComps.erase(iter);
    }
}


// ビルボードコンポーネント登録
void Renderer::AddBillboardComp(BillboardComponent* billboard)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int drawOrder = billboard->GetDrawOrder();
    auto iter = billboardComps.begin();
    for (;iter != billboardComps.end(); ++iter)
    {
        if (drawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    billboardComps.insert(iter, billboard);
    
    
}

// パーティクルコンポーネント登録
void Renderer::RemoveBillboardComp(BillboardComponent* billboard)
{
    auto iter = std::find(billboardComps.begin(), billboardComps.end(), billboard);
    if (iter != billboardComps.end())
    { // 要素が見つかった場合のみ削除
        billboardComps.erase(iter);
    }
}




// デバッガーコンポーネント登録
void Renderer::AddDebuggerComp(DebuggerComponent* dbg)
{
    dbgComps.emplace_back(dbg);
}

// パーティクルコンポーネント登録
void Renderer::RemoveDebuggerComp(DebuggerComponent* dbg)
{
    auto iter = std::find(dbgComps.begin(), dbgComps.end(), dbg);
    if (iter != dbgComps.end())
    { // 要素が見つかった場合のみ削除
        dbgComps.erase(iter);
    }
}



// データ解放
void Renderer::UnloadData()
{
    // テクスチャ削除
    textures.clear();
    
    // メッシュ削除
    meshes.clear();
    
}
