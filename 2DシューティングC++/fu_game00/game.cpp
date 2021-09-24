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
#include "scene2D.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "time.h"
#include "boss.h"
#include "mesh.h"
#include "move.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PLAYER_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))
#define PLAYER_POS		(D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, 0.0f))

#define SCORE_POS		(D3DXVECTOR3(100.0f, 100.0f, 0.0f))
#define SCORE_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))

#define FIELD_POS		(D3DXVECTOR3(0.0f, 668.0f, 0.0f))
#define FIELD_SIZE		(D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f))
#define FIELD_VERTICAL	(100)
#define FIELD_SIDE		(0)

#define TIME_POS		(D3DXVECTOR3(SCREEN_WIDTH / 3, 50.0f, 0.0f))
#define TIME_SIZE		(D3DXVECTOR3(20.0f, 30.0f, 0.0f))
#define TIME_SET		(100)

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

	// 初期化処理
	pGame->Init();

	return pGame;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGame::Init(void)
{
	m_pBg = CScene2D::Create(CENTER_POS, BG_SIZE);


	m_pField = CMesh::Create(FIELD_VERTICAL, FIELD_SIDE, FIELD_POS, FIELD_SIZE);
	m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE);

	m_pScore = CScore::Create(SCORE_POS, SCORE_SIZE);
	m_pTime = CTime::Create(TIME_POS, TIME_SIZE, TIME_SET);
	m_pBoss = CBoss::Create(BOSS_POS, BOSS_SIZE, BOSS_LIFE);

	m_pBoss->CreateTexture(BOSS_TEXTURE);
	m_pBg->CreateTexture(BG_TEXTURE);
	m_pField->CreateTexture(FIELD_TEXTURE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	CKey *pKey = CManager::GetKey();

	m_fWaveTime += 0.02f;

	for (int nVtx = 0; nVtx < m_pField->GetVtxNum()/2; nVtx++)
	{
		D3DXVECTOR3 pos = ZeroVector3;
		pos.y = CMove::SinWave(670.0f, 25.0f, 70.0f, (m_fWaveTime + nVtx));
		m_pField->SetWavePos(nVtx, pos.y);
	}

	/* フェード処理 */
	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE))
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}
	if (m_pTime->GetTime() == 0 || CBoss::GetAlive() == false)
	{
		CManager::GetFade()->SetFade(CManager::MODE_RESULT);
	}

	/* スコアの加算 */
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_Y))
	{
		m_pScore->AddScore(12);
	}

	/* タイマー処理 */
	int nCntTime = m_pTime->AddCnt(1);

	if ((nCntTime % 60) == 0)
	{// タイマーの減算
		m_pTime->AddTime(-1);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	
}
