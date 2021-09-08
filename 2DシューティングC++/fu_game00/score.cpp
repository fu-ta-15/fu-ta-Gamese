//*****************************************************************************
//
// スコア処理 [score.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "score.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define POLYGON_POS
#define POLYGON_SIZE

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore() : CScene(OBJ_NONE)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{
}

//=============================================================================
// スコアの生成
//=============================================================================
CScore * CScore::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{
		pScore = new CScore;
		pScore->m_pos = pos;
		pScore->m_size = size;
		pScore->m_nScore = 0;
		pScore->Init();
	}

	return pScore;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CScore::Init()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{/* 右に位置を進める　＝　左か 100・10・1 という桁 */
		m_apNumber[nCnt] = CNumber::Create(m_pos, m_size, 10.0f);
		m_apNumber[nCnt]->Load("data/TEXTURE/number000.png");
		m_pos.x += m_size.x * 2;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apNumber[nCnt]->Uninit();
		delete m_apNumber[nCnt];
		m_apNumber[nCnt] = NULL;
	}

	//オブジェクトの破棄
	Release();

}

//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apNumber[nCnt]->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apNumber[nCnt]->Draw();
	}
}

//=============================================================================
// スコアのセット
//=============================================================================
void CScore::SetScore(int nScore)
{
	int aNumber[MAX_SCORE];
	int nMath = 1000;	//スコア計算


	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		aNumber[nCnt] = (nScore % nMath / (nMath / 10));
		m_apNumber[nCnt]->SetNumber(aNumber[nCnt]);
		nMath /= 10;
	}
}

//=============================================================================
// スコアの加算
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;
	SetScore(m_nScore);
}

//=============================================================================
// 現在のスコアの所得
//=============================================================================
int CScore::GetScore()
{
	return m_nScore;
}
