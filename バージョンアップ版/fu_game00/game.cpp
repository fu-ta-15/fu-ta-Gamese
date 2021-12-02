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
#include "mesh3D.h"

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
CMesh*		CGame::m_pField		= NULL;
CMesh3D*	CGame::m_pBg3D		= NULL;

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
	m_nWaveCnt = 0;

	// 3D背景の生成
	m_pBg3D = CMesh3D::Create(40, 40, D3DXVECTOR3(-40.0f, 0.0f, 0.0f), D3DXVECTOR3(400.0f, 0.0f, 200.0f));

	// 3D背景のテクスチャ設定
	m_pBg3D->CreateTexture("data/TEXTURE/BG.jpg");
	
	// 地面の生成
	m_pField = CMesh::Create(FIELD_VERTICAL, 0, FIELD_POS, FIELD_SIZE,CScene::OBJ_NONE2);

	// 地面のテクスチャ
	m_pField->CreateTexture(FIELD_TEXTURE);
	
	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(PLAYER_POS, PLAYER_SIZE);
	
	// タイムの生成 *裏で回しているためポリゴンとしては表示しない
	m_pTime = CTime::Create(ZeroVector3, ZeroVector3, 0);
	
	// ボスの生成
	m_pBoss = CBoss::Create(BOSS_POS, BOSS_SIZE, BOSS_LIFE);

	// ボスのテクスチャ
	m_pBoss->CreateTexture(BOSS_TEXTURE);

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

	// 波の表現
	m_pBg3D->XSideWave(D3DXVECTOR3(0.0f,0.0f,-100.0f), (int)m_nWaveCnt, 10, 25);

	// ウェーブのカウント
	m_nWaveCnt++;

	// 時間のカウントアップ
	if ((m_pTime->AddCnt(1) % 60) == 0) { m_pTime->AddTime(1); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	
}
