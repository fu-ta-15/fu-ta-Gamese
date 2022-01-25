//=============================================================================
//
// モード選択画面処理 [mode_select.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "mode_select.h"
#include "manager.h"
#include "fade.h"
#include "sound.h"
#include "set_mode_select.h"

#include "preparation_player.h"
#include "game.h"
#include "tutorial.h"
#include "debugproc.h"

//=============================================================================
// CModeSelectのコンストラクタ
//=============================================================================
CModeSelect::CModeSelect()
{

}

//=============================================================================
// CModeSelectのデストラクタ
//=============================================================================
CModeSelect::~CModeSelect()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModeSelect::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_modeselect = MODESELECT_NORMAL;

	// タイトル情報の読み込み
	CSetModeSelect *pSet = new CSetModeSelect;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// サウンド
	CSound::Play(CSound::SOUND_LABEL_MODESELECT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModeSelect::Uninit(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void CModeSelect::Update(void)
{
	// 画面遷移
	CBase *pBase = NULL;
	CPreparationPlayer *pPP = NULL;

	switch (m_modeselect)
	{
		// 通常
	case MODESELECT_NORMAL:
		break;
		// シングルモード遷移
	case MODESELECT_TRANSITION_GAME_BATTLE:
		pPP = new CPreparationPlayer;
		pPP->SetMode(CPreparationPlayer::PREPARATIONPLAYER_TRANSITION_GAME_BATTLE);
		pBase = pPP;

		if (pBase != NULL)
		{
			CManager::SetMode(pBase);
		}
		break;
		// マルチモード遷移
	case MODESELECT_TRANSITION_GAME_COOPERATION:
		pPP = new CPreparationPlayer;
		pPP->SetMode(CPreparationPlayer::PREPARATIONPLAYER_TRANSITION_GAME_COOPERATION);
		pBase = pPP;

		if (pBase != NULL)
		{
			CManager::SetMode(pBase);
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CModeSelect::Draw(void)
{

}