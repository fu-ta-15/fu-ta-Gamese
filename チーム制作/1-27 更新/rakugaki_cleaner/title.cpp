//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "fade.h"

#include "set_title.h"
#include "mode_select.h"
#include "title_manager.h"

#include "debugproc.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CTitleManager *CTitle::m_pTitleManager = NULL;

//=============================================================================
// CTitleのコンストラクタ
//=============================================================================
CTitle::CTitle()
{
	// メンバ変数の初期化
	m_titlemode = TITLEMODE_NORMAL;
	m_pTitleManager = NULL;
}

//=============================================================================
// CTitleのデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_titlemode = TITLEMODE_NORMAL;
	m_pTitleManager = NULL;

	// タイトル管理情報の生成
	m_pTitleManager = new CTitleManager;

	if (m_pTitleManager != NULL)
	{
		m_pTitleManager->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
	}

	// タイトル情報の読み込み
	CSetTitle *pSet = new CSetTitle;

	if (pSet != NULL)
	{
		pSet->Init();
	}

	// サウンド
	CSound::Play(CSound::SOUND_LABEL_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// サウンドを止める
	CSound::Stop();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 画面遷移
	switch (m_titlemode)
	{
		// 通常
	case TITLEMODE_NORMAL:
		break;
		// 遷移
	case TITLEMODE_TRANSITION:
		CModeSelect *pModeSelect = new CModeSelect;

		if (pModeSelect != NULL)
		{
			CManager::GetFade()->SetFade(CFade::FADE_OUT, pModeSelect);
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}