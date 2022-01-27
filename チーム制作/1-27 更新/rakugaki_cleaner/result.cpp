//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "input_keyboard.h"
#include "fade.h"
#include "input_joypad.h"
#include "sound.h"
#include "stage.h"
#include "motion.h"
#include "camera.h"
#include "player_result.h"
#include "keep_object.h"
#include "player_marker.h"
#include "rank.h"
#include "texture.h"
#include "ranking_namespace.h"
#include "result_manager.h"
#include "score.h"
#include "number.h"
#include "texture.h"
#include "result_ui.h"
#include "set_result.h"

#include "title.h"
#include "game.h"
#include "mode_select.h"

#include "debugproc.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPlayer_Result **CResult::m_pPlayer = NULL;
CCamera *CResult::m_pCamera = NULL;
CResult::ResultMode CResult::m_resultMode = RESULTMODE_NORMAL;

//=============================================================================
// CGameのコンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_resultMode = RESULTMODE_NORMAL;
	m_pResult_UI = NULL;

	// テクスチャ管理情報の取得
	CTexture *pTex = CManager::GetTexture();

	// データ保存用オブジェクトの取得
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	// 順位の確定
	pKeepObject->Rank();

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
		m_pCamera= CCamera::Create(D3DXVECTOR3(0.0f, 35.0f, -120.0f), D3DXVECTOR3(0.0f, 30.0f, 80.0f), viewport);
		m_pCamera->SetParallel(D3DXVECTOR2(160.0f, 90.0f));
	}

	// プレイヤーの生成
	int nNumPlayer = pKeepObject->GetNumPlayer();

	// プレイ人数ごとにメモリを確保
	m_pPlayer = new CPlayer_Result*[nNumPlayer];

	for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
	{
		// 順位の取得
		int nRank = pKeepObject->GetRank(nCnt) - 1;			// ランク
		int nNumRank = pKeepObject->GetNumRank(nCnt) - 1;	// 位置

		if (nNumPlayer >= 2)
		{
			// プレイヤーの配置
			m_pPlayer[nCnt] = CPlayer_Result::Create(D3DXVECTOR3(-50.0f + (((50.0f * 2.0f) / (float)(nNumPlayer - 1)) * nNumRank), 0.0f, 0.0f), D3DXVECTOR2(20.0f, 100.0f), nCnt);
		}
		else
		{
			m_pPlayer[nCnt] = CPlayer_Result::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(20.0f, 100.0f), nCnt);
		}

		// プレイヤーマーカーの配置
		m_pPlayer[nCnt]->SetPlayerMarker(CPlayerMarker::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(60.0f, 40.0f), D3DXVECTOR2(4, 1), pTex->SetTexture("data/TEXTURE/player_Logo00.png")), m_pCamera);
	}

	if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_COOPERATION)
	{
		m_pResult_UI = new CResult_UI;

		if (m_pResult_UI != NULL)
		{
			m_pResult_UI->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });

			for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
			{
				if (m_pResult_UI->GetAchivement())
				{
					m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_VICTORY);
				}
				else
				{
					m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_LOSE);
				}
			}
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < nNumPlayer; nCnt++)
		{
			// 順位の取得
			int nRank = pKeepObject->GetRank(nCnt) - 1;			// ランク

			// 順位でモーションを変更
			if (nRank == 0)
			{
				m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_VICTORY);
			}
			else
			{
				m_pPlayer[nCnt]->SetMotion(CPlayer::MOTIONTYPE_LOSE);
			}

			// ランキングロゴの配置
			m_pPlayer[nCnt]->SetRank(CRank::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(90.0f, 130.0f), D3DXVECTOR2(4, 1), pTex->SetTexture("data/TEXTURE/Ranking00.png")), nRank, m_pCamera);
		}
	}
	// モーションデータの読み込み
	CMotion::LoadMotionData();

	// タイトル情報の読み込み
	CSetResult *pSet = new CSetResult;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// ステージの配置
	CStage::Create(D3DXVECTOR3(0.0f, -402.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/table.x");

	// リザルト管理の生成
	m_pResultManager = new CResultManager;

	if (m_pResultManager != NULL)
	{
		m_pResultManager->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	}

	// サウンド
	CSound::Play(CSound::SOUND_LABEL_RESULT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// サウンド
	CSound::Stop();

	// カメラの終了処理
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
void CResult::Update(void)
{
	CBase *pBase = NULL;

	switch (m_resultMode)
	{
		// 通常
	case RESULTMODE_NORMAL:
		break;
		// タイトルに遷移
	case RESULTMODE_RESTART:
		pBase = new CGame;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
		// ゲームに遷移
	case RESULTMODE_TRANSITION_MODESELECT:
		pBase = new CModeSelect;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
		// リザルトに遷移
	case RESULTMODE_TRANSITION_TITLE:
		pBase = new CTitle;

		if (pBase != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
		}
		break;
	}
	m_pCamera->Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{	
	// カメラの配置
	m_pCamera->SetCamera();
}