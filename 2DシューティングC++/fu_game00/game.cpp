//*****************************************************************************
//
// ゲーム処理 [game.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "game.h"
#include "result.h"

#include "manager.h"
#include "sound.h"
#include "fade.h"
#include "keyinput.h"

#include "enemy.h"
#include "boss.h"
#include "player.h"

#include "scene2D.h"
#include "mesh.h"

#include "time.h"
#include "move.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PLAYER_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define PLAYER_POS		(D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 0.0f))

#define SCORE_POS		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))
#define SCORE_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))

#define FIELD_POS		(D3DXVECTOR3(0.0f, 668.0f, 0.0f))
#define FIELD_SIZE		(D3DXVECTOR3(WIDTH_HALF+100.0f, 100.0f, 0.0f))
#define FIELD_VERTICAL	(100)

#define BG_POS			(CENTER_POS)
#define BG_SIZE			(D3DXVECTOR3(WIDTH_HALF,HEIGHT_HALF,0.0f))

#define BG_TEXTURE		("data/TEXTURE/BG.png")
#define BOSS_TEXTURE	("data/TEXTURE/Boss.png")
#define FIELD_TEXTURE	("data/TEXTURE/Field.png")

//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CPlayer*	CGame::m_pPlayer	= NULL;
CScore*		CGame::m_pScore		= NULL;
CTime*		CGame::m_pTime		= NULL;
CBoss*		CGame::m_pBoss		= NULL;
CScene2D*	CGame::m_pBg		= NULL;
CMesh*		CGame::m_pField		= NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame() : CScene(OBJ_NONE)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
}

//=============================================================================
// ゲームの生成
//=============================================================================
CGame * CGame::Create(void)
{
	// メモリ確保
	CGame* pGame = new CGame;
	
	// NULLチェック
	if (pGame != NULL)
	{
		// 初期化処理
		pGame->Init();
	}

	return pGame;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGame::Init(void)
{
	// サウンドクラスのポインタ
	CSound *pSound = CManager::GetSound();

	// 波を起こすためのカウント
	m_nWaveCnt = 0.0f;

	// 波の高さ
	m_fWaveHeight = 20.0f;

	// 背景の生成
	m_pBg = CScene2D::Create(BG_POS, BG_SIZE);

	// ダメージ表現用のメッシュポリゴン生成
	m_pLifeMesh = CMesh::Create(100, 0, D3DXVECTOR3(0.0f,HEIGHT_HALF,0.0f), D3DXVECTOR3(SCREEN_WIDTH, 5.0f, 0.0f), CScene::OBJ_NONE2);
	
	// 地面の生成
	m_pField = CMesh::Create(FIELD_VERTICAL, 0, FIELD_POS, FIELD_SIZE,CScene::OBJ_NONE2);
	
	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE);
	
	// タイムの生成 *裏で回しているためポリゴンとしては表示しない
	m_pTime = CTime::Create(ZeroVector3, ZeroVector3, 0);
	
	// ボスの生成
	m_pBoss = CBoss::Create(BOSS_POS, BOSS_SIZE, BOSS_LIFE);

	// ボスのテクスチャ
	m_pBoss->CreateTexture(BOSS_TEXTURE);

	// 背景のテクスチャ
	m_pBg->CreateTexture(BG_TEXTURE);

	// 地面のテクスチャ
	m_pField->CreateTexture(FIELD_TEXTURE);

	// サウンドの開始
	pSound->PlaySound(CSound::SOUND_LABEL_BGM002);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// サウンドクラスのポインタ
	CSound *pSound = CManager::GetSound();

	// サウンドの停止
	pSound->StopSound();

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	// キー入力クラスのポインタ
	CKey *pKey = CManager::GetKey();

	// プレイヤーが死亡したら
	if (m_pPlayer->GetAlive() == false)
	{
		// リザルトに結果を報告
		CResult::GameEndResult(false);

		// リザルトシーンに移動
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

	// ボスが死亡したら
	if (m_pBoss->GetAlive() == false)
	{
		// リザルトに結果を報告
		CResult::GameEndResult(true);

		// リザルトシーンに移動
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

	// ウェーブのカウント
	m_nWaveCnt += 0.52f;

	for (int nVtx = 0; nVtx < m_pLifeMesh->GetVtxNum() / 2; nVtx++)
	{
		// 座標保管用
		D3DXVECTOR3 pos = ZeroVector3;

		// ボスがダメージを受けたら
		if (m_pBoss->GetState() == CBoss::STATE_NOT_DAMAGE)
		{
			m_fWaveHeight = (float)(rand() % 300 - 200);
			m_nWaveCnt += rand() % 15 + 2;
			pos.y = Move::CosWave(HEIGHT_HALF, m_fWaveHeight, 320.0f, (m_nWaveCnt) + nVtx);
		}
		else
		{
			m_fWaveHeight += (20.0f - m_fWaveHeight) * 0.0025f;
			pos.y = Move::CosWave(HEIGHT_HALF, m_fWaveHeight, 240.0f, (m_nWaveCnt) + nVtx);
		}

		m_pLifeMesh->SetVtxPosY(nVtx, pos.y);
		pos.y += 5;
		m_pLifeMesh->SetVtxPosY(nVtx + (m_pLifeMesh->GetVtxNum() / 2), pos.y);
	}

	/* タイマー処理 */
	if ((m_pTime->AddCnt(1) % 60) == 0)
	{
		m_pTime->AddTime(1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	
}
