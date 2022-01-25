//=============================================================================
//
// プレイヤーマーカー処理 [player_marker.cpp]
// Author : 
//
//=============================================================================
#include "player_marker.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CPlayerMarkerのコンストラクタ
//=============================================================================
CPlayerMarker::CPlayerMarker(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CPlayerMarkerのデストラクタ
//=============================================================================
CPlayerMarker::~CPlayerMarker()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayerMarker *CPlayerMarker::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	// ポインタ変数の生成
	CPlayerMarker *pPlayerMarker;
	pPlayerMarker = new CPlayerMarker(CScene::PRIORITY_UI);

	// NULLチェック
	if (pPlayerMarker != NULL)
	{
		// 初期化処理
		pPlayerMarker->Init(pos, size, texSheet, nTex);

		// オブジェクトの種類を設定
		pPlayerMarker->SetObjType(CScene::OBJTYPE_PLAYER_MARKER);
	}

	return pPlayerMarker;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayerMarker::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	// CScene2Dの初期化処理
	CScene2D::Init(pos,size);

	// テクスチャの貼り付け
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		CScene2D::BindTexture(pTex->GetAddress(nTex));
	}

	// テクスチャの分割量の設定
	CScene2D::SetTexSheet(texSheet);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayerMarker::Uninit(void)
{
	// CScene2Dの終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayerMarker::Update(void)
{
	// CScene2Dの更新処理
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayerMarker::Draw(void)
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