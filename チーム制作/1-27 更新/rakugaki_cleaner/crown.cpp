//=============================================================================
//
// 王冠処理 [crown.cpp]
// Author : 
//
//=============================================================================
#include "crown.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CCrownのコンストラクタ
//=============================================================================
CCrown::CCrown(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CCrownのデストラクタ
//=============================================================================
CCrown::~CCrown()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CCrown *CCrown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// ポインタ変数の生成
	CCrown *pCrown;
	pCrown = new CCrown(CScene::PRIORITY_UI);

	// NULLチェック
	if (pCrown != NULL)
	{
		// 初期化処理
		pCrown->Init(pos, size, nTex);

		// オブジェクトの種類を設定
		pCrown->SetObjType(CScene::OBJTYPE_PLAYER_MARKER);
	}

	return pCrown;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCrown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos, size);

	// テクスチャの貼り付け
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		CScene2D::BindTexture(pTex->GetAddress(nTex));
	}

	m_bDisplay = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCrown::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCrown::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CCrown::Draw(void)
{
	if (m_bDisplay == true)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// アルファテスト
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

		// CScene2Dの描画処理
		CScene2D::Draw();

		// アルファテストを元の設定に戻す
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
	}
}