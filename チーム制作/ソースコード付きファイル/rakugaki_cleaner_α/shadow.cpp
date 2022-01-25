//=============================================================================
//
// スキル処理 [skill.cpp]
// Author : 
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "debugproc.h"

//=============================================================================
// CShadowのコンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority) : CScene3D(nPriority)
{
	// 変数の初期化
}

//=============================================================================
// CShadowのデストラクタ
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CShadow *pShadow;
	pShadow = new CShadow(CScene::PRIORITY_SHADOW);

	// NULLチェック
	if (pShadow != NULL)
	{
		// 初期化処理
		pShadow->Init(pos, size);

		// オブジェクトタイプの設定
		pShadow->SetObjType(OBJTYPE_SHADOW);
	}

	return pShadow;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScene2Dの初期化処理
	CScene3D::Init(pos, size);

	// テクスチャの設定
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(pTex->SetTexture("data/TEXTURE/shadow000.jpg")));
	}

	SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	// CScene2Dの終了処理
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CShadow::Update(void)
{
	// CScene2Dの更新処理
	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CShadow::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// CScene2Dの描画処理
	CScene3D::Draw();

	// 通常の設定に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}