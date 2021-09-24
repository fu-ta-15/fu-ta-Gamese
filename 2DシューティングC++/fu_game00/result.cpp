//*****************************************************************************
//
// リザルト処理 [result.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "result.h"
#include "keyinput.h"
#include "scene2D.h"
#include "manager.h"
#include "fade.h"
#include "score.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define RANKING_POS				(D3DXVECTOR3(WIDTH_HALF,HEIGHT_HALF-300.0f,0.0f))
#define RANKING_SIZE			(D3DXVECTOR3(20.0f,30.0f,0.0f))

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CScore*		CResult::m_paRanking[MAX_RANKING] = {};


//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult() : CScene(OBJ_NONE)
{
	m_pos = RANKING_POS;
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{
}

//=============================================================================
// リザルトの生成
//=============================================================================
CResult * CResult::Create(void)
{
	// メモリ確保
	CResult* pResult = new CResult;

	// 初期化処理
	pResult->Init();

	return pResult;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CResult::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{// ランキングポリゴン生成
		m_paRanking[nCnt] = CScore::Create(m_pos, RANKING_SIZE);
		m_pos.y += 100.0f;	// Y座標の間隔をあける
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	CKey *pKey = CManager::GetKey();

	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE))
	{
		CManager::GetFade()->SetFade(CManager::MODE_TITLE);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
}
