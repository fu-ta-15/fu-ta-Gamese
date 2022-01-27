//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"

#include "input_keyboard.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "player_Game.h"
#include "motion.h"
#include "stage.h"
#include "pause.h"
#include "countdown.h"
#include "graffiti_manager.h"
#include "gauge_manager.h"
#include "player_marker.h"
#include "eraser.h"
#include "keep_object.h"

#include "set_model.h"
#include "set_graffiti.h"
#include "set_game.h"

#include "title.h"
#include "mode_select.h"
#include "result.h"

#include "debugproc.h"
#include "texture.h"
#include "camera.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CTimer *CGame::m_pTimer = NULL;
CPlayer_Game **CGame::m_pPlayer = NULL;
CStage *CGame::m_pStage = NULL;
CSetGame *CGame::m_pLoadData = NULL;
CCountdown *CGame::m_pCountdown = NULL;
CGraffitiManager *CGame::m_pGraffitiManager = NULL;
CGaugeManager *CGame::m_pGaugeManager = NULL;
int CGame::m_nCntPlayerOP = 0;
CGame::GAMEMODE CGame::m_gamemode = GAMEMODE_NORMAL;
CCamera *CGame::m_pCamera = NULL;

//=============================================================================
// CGameのコンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// カメラの生成
	m_pCamera = new CCamera;

	if (m_pCamera != NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = SCREEN_WIDTH;
		viewport.Height = SCREEN_HEIGHT;
		viewport.MinZ = 0;
		viewport.MaxZ = 1;

		m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 482.0f, -200.0f), D3DXVECTOR3(0.0f, -40.0f, 0.0f), viewport);
	}

	// ゲームモード
	m_gamemode = GAMEMODE_NORMAL;

	// プレイヤーの生成
	CKeepObject *pKeepOBject = CManager::GetKeepObject();

	m_nNumPlayer =pKeepOBject->GetNumPlayer();
	m_pPlayer = new CPlayer_Game*[m_nNumPlayer];

	for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
	{
		if (m_nNumPlayer >= 2)
		{
			m_pPlayer[nCnt] = CPlayer_Game::Create(D3DXVECTOR3(-50.0f + (((50.0f * 2.0f) / (float)(m_nNumPlayer - 1)) * nCnt), 0.0f, 0.0f), D3DXVECTOR2(20.0f, 100.0f), nCnt);
		}
		else
		{
			m_pPlayer[nCnt] = CPlayer_Game::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(20.0f, 100.0f), nCnt);
		}
		// プレイヤーマーカーの初期化
		CTexture *pTex = CManager::GetTexture();
		m_pPlayer[nCnt]->SetPlayerMarker(CPlayerMarker::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(60.0f, 40.0f), D3DXVECTOR2(4, 1), pTex->SetTexture("data/TEXTURE/player_Logo00.png")),m_pCamera);

		// 消しゴムの生成
		m_pPlayer[nCnt]->SetEraser(CEraser::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.5f, 100.0f)));
	}

	// ラクガキ管理クラスの生成
	m_pGraffitiManager = new CGraffitiManager;

	if (m_pGraffitiManager != NULL)
	{
		m_pGraffitiManager->Init(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR2(0.0f,0.0f));
	}

	// ゲージ管理クラスの生成
	m_pGaugeManager = new CGaugeManager;

	if (m_pGaugeManager != NULL)
	{
		m_pGaugeManager->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// データ読み込みクラスのインスタンス生成
	m_pLoadData = new CSetGame;

	if (m_pLoadData != NULL)
	{
		m_pLoadData->Init();
	}

	// モーションデータの読み込み
	CMotion::LoadMotionData();

	// ラクガキ配置情報の読み込み
	CSetGraffiti::LoadGraffitiData();

	// 現在のラクガキ数を変更
	m_pGraffitiManager->SetCurGraffiti(m_pGraffitiManager->GetTotalGraffiti());

	// ポーズの生成
	CPause::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));

	// カウントダウンの生成
	m_pCountdown = CCountdown::Create(D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR2(0.0f,0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// サウンド停止
	CSound::Stop();

	// カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	// モードによって処理を区別
	CBase *pBase = NULL;

	switch (m_gamemode)
	{
		// 通常
	case GAMEMODE_NORMAL:
		break;
		// タイトルに遷移
	case GAMEMODE_TRANSITION_MODESELECT:
		pBase = new CModeSelect;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
		// ゲームに遷移
	case GAMEMODE_TRANSITION_GAME:
		pBase = new CGame;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
		// リザルトに遷移
	case GAMEMODE_TRANSITION_RESULT:
		pBase = new CResult;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
	}

#ifdef _DEBUG
	// キーボード情報の取得
	CInputKeyboard *pKey = GET_KEYBOARD;

	// 操作可能プレイヤーの操作
	if (pKey->GetTrigger(KEY_PLAYEROP_UP))
	{
		m_nCntPlayerOP++;

		if (m_nCntPlayerOP > m_nNumPlayer - 1)
		{
			m_nCntPlayerOP = 0;
		}
	}
	else if (pKey->GetTrigger(KEY_PLAYEROP_DOWN))
	{
		m_nCntPlayerOP--;

		if (m_nCntPlayerOP < 0)
		{
			m_nCntPlayerOP = m_nNumPlayer - 1;
		}
	}
	CDebugProc::Print("操作可能プレイヤー : (%d / %d)\n", m_nCntPlayerOP, m_nNumPlayer - 1);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{
	// カメラのセット
	m_pCamera->SetCamera();
}