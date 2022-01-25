//=============================================================================
//
// タイトルロゴ処理 [title_logo.cpp]
// Author : 
//
//=============================================================================
#include "title_logo.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "title.h"

//=============================================================================
// CTitleLogoのコンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CTitleLogoのデストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// ポインタ変数の生成
	CTitleLogo *pTitleLogo;
	pTitleLogo = new CTitleLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pTitleLogo != NULL)
	{
		// 初期化処理
		pTitleLogo->Init(pos, size, nTex);

		// オブジェクトタイプの設定
		pTitleLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pTitleLogo;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
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
void CTitleLogo::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitleLogo::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{
	// CScene2Dの描画処理
	CScene2D::Draw();
}