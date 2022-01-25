//=============================================================================
//
// データ保存用処理 [keeobject.cpp]
// Author : 
//
//=============================================================================
#include "keep_object.h"
#include "manager.h"
#include "renderer.h"

#include "load_player_data.h"
#include "motion.h"
#include "ranking_namespace.h"
#include "debugproc.h"

//=============================================================================
// CKeepObjectのコンストラクタ
//=============================================================================
CKeepObject::CKeepObject()
{

}

//=============================================================================
// CDebugProcのデストラクタ
//=============================================================================
CKeepObject::~CKeepObject()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CKeepObject::Init(void)
{
	// プレイヤーデータ読み込み情報の初期化
	m_pLoadPlayerData = new CLoadPlayerData;

	if (m_pLoadPlayerData != NULL)
	{
		m_pLoadPlayerData->Init();
	}

	m_gamemode = GAMEMODE_BATTLE;
}

//=============================================================================
// 終了処理
//=============================================================================
void CKeepObject::Uninit(void)
{
	// メモリの破棄
	DeleteMem();

	// プレイヤーデータ読み込み情報の破棄
	if (m_pLoadPlayerData != NULL)
	{
		m_pLoadPlayerData->Uninit();
		delete m_pLoadPlayerData;
		m_pLoadPlayerData = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CKeepObject::Update(void)
{

}

//=============================================================================
// プレイ人数を設定
//=============================================================================
void CKeepObject::SetNumPlayer(int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;

	// メモリの確保
	m_nNumDeleteGraffiti = new int[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_nNumDeleteGraffiti[nCnt] = 0;
	}
}

//=============================================================================
// メモリの破棄
//=============================================================================
void CKeepObject::DeleteMem(void)
{
	// 消したラクガキ数を保存するポインタの破棄
	if (m_nNumDeleteGraffiti != NULL)
	{
		delete[] m_nNumDeleteGraffiti;
		m_nNumDeleteGraffiti = NULL;
	}

	// 順位を保存するポインタの破棄
	if (m_nRank != NULL)
	{
		delete[] m_nRank;
		m_nRank = NULL;
	}

	// 順位を保存するポインタの破棄
	if (m_nNumberRank != NULL)
	{
		delete[] m_nNumberRank;
		m_nNumberRank = NULL;
	}
}

//=============================================================================
// ランキングの確定
//=============================================================================
void CKeepObject::Rank(void)
{
	// メモリの確保
	m_nRank = new int[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_nRank[nCnt] = 0;
	}

	m_nNumberRank = new int[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		m_nNumberRank[nCnt] = 0;
	}

	// 順位付け
	RankingSpace::SetRanking(&m_nRank[0], &m_nNumDeleteGraffiti[0], m_nNumPlayer);
	RankingSpace::SetSameRate(&m_nNumberRank[0], &m_nNumDeleteGraffiti[0], m_nNumPlayer);
}