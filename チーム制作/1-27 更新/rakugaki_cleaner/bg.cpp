//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : 
//
//=============================================================================
#include "bg.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CBgのコンストラクタ
//=============================================================================
CBg::CBg(int nPriority) : CScene2D(nPriority)
{
	// 変数の初期化
	m_tex = D3DXVECTOR2(0.0f, 0.0f);
	m_moveTex = D3DXVECTOR2(0.0f, 0.0f);
}

//=============================================================================
// CBgのデストラクタ
//=============================================================================
CBg::~CBg()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, D3DXVECTOR2 moveTex, int nTex)
{
	// ポインタ変数の生成
	CBg *pBg;
	pBg = new CBg(CScene::PRIORITY_BG);

	// NULLチェック
	if (pBg != NULL)
	{
		// 初期化処理
 		pBg->Init(pos, size,col, moveTex, nTex);

		// オブジェクトタイプの設定
		pBg->SetObjType(OBJTYPE_BG);
	}

	return pBg;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, D3DXVECTOR2 moveTex, int nTex)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// 色の指定
	SetCol(col);

	// テクスチャ管理情報の取得
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(nTex));
	}

	// メンバ変数の初期化
	m_tex = D3DXVECTOR2(0.0f, 1.0f);
	m_moveTex = moveTex;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBg::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBg::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();

	// テクスチャ座標の移動
	MoveTex();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBg::Draw(void)
{
	// 変数の受け渡し
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Zバッファを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// CScene2Dの描画処理
	CScene2D::Draw();

	// 標準に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// テクスチャ座標移動処理
//=============================================================================
void CBg::MoveTex(void)
{
	// テクスチャ座標の更新
	m_tex += m_moveTex;

	// テクスチャ座標の設定
	SetTex(D3DXVECTOR2(m_tex.x, m_tex.y));
}