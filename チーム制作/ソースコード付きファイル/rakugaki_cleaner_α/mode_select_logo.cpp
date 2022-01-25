//=============================================================================
//
// モード選択ロゴ処理 [mode_select_logo.cpp]
// Author : 
//
//=============================================================================
#include "mode_select_logo.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CModeSelectLogoのコンストラクタ
//=============================================================================
CModeSelectLogo::CModeSelectLogo(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CModeSelectLogoのデストラクタ
//=============================================================================
CModeSelectLogo::~CModeSelectLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CModeSelectLogo *CModeSelectLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// ポインタ変数の生成
	CModeSelectLogo *pModeSelectLogo;
	pModeSelectLogo = new CModeSelectLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pModeSelectLogo != NULL)
	{
		// 初期化処理
		pModeSelectLogo->Init(pos, size, nTex);

		// オブジェクトタイプの設定
		pModeSelectLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pModeSelectLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModeSelectLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// テクスチャ管理情報の取得
	CTexture *pTex = CManager::GetTexture();

	// 情報がNULLじゃないときテクスチャを適用する
	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(nTex));
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModeSelectLogo::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CModeSelectLogo::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CModeSelectLogo::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}