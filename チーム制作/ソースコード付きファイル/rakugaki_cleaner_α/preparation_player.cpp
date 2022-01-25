//=============================================================================
//
// プレイヤー待機画面処理 [mode_select.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "preparation_player.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "set_preparation_player.h"
#include "preparation_player_manager.h"
#include "mode_select.h"
#include "input_keyboard.h"
#include "keep_object.h"

#include "game.h"
#include "tutorial.h"
#include "debugproc.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CPreparationPlayerManager *CPreparationPlayer::m_pPPM = NULL;
int CPreparationPlayer::m_nCntPlayerOP = 0;

//=============================================================================
// CPreparationPlayerのコンストラクタ
//=============================================================================
CPreparationPlayer::CPreparationPlayer()
{
	// メンバ変数の初期化
	m_pPPM = NULL;
}

//=============================================================================
// CPreparationPlayerのデストラクタ
//=============================================================================
CPreparationPlayer::~CPreparationPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPreparationPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_nCntPlayerOP = 0;
	m_nNumPlayer = 4;
	// プレイヤー準備画面管理情報の生成
	m_pPPM = new CPreparationPlayerManager;

	if (m_pPPM != NULL)
	{
		m_pPPM->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPreparationPlayer::Uninit(void)
{
	// サウンドの停止
	CSound::Stop();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPreparationPlayer::Update(void)
{
	// 画面遷移
	CBase *pBase = NULL;
	CKeepObject *pKeepObject = CManager::GetKeepObject();

	if (m_pPPM->GetTransition() == true)
	{
		switch (m_preparationPlayer)
		{
			// 通常
		case PREPARATIONPLAYER_NORMAL:
			break;
			// シングルモード遷移
		case PREPARATIONPLAYER_TRANSITION_GAME_BATTLE:
			pKeepObject->SetGameMode(CKeepObject::GAMEMODE_BATTLE);
			pBase = new CGame;

			if (pBase != NULL)
			{
				CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
			}
			break;
			// マルチモード遷移
		case PREPARATIONPLAYER_TRANSITION_GAME_COOPERATION:
			pKeepObject->SetGameMode(CKeepObject::GAMEMODE_COOPERATION);
			pBase = new CGame;

			if (pBase != NULL)
			{
				CManager::GetFade()->SetFade(CFade::FADE_OUT, pBase);
			}
			break;
			// モーで選択
		case PREPARATIONPLAYER_TRANSITION_MODESELECT:
			pBase = new CModeSelect;

			if (pBase != NULL)
			{
				CManager::SetMode(pBase);
			}
			break;
		}
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
void CPreparationPlayer::Draw(void)
{
	// プレイヤー準備画面管理情報の描画処理
	m_pPPM->Draw();
}