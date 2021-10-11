//*****************************************************************************
//
// マネージャー処理 [manager.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "fade.h"
#include "keyinput.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "pause.h"
#include "sound.h"

//-----------------------------------------------------------------------------
//静的メンバ変数宣言
//-----------------------------------------------------------------------------
CRenderer *CManager::m_pRenderer = NULL;

CKey *CManager::m_pKey = NULL;

CManager::MODE CManager::m_mode = CManager::MODE_TITLE;

CFade *CManager::m_pFade = NULL;

CTitle *CManager::m_pTitle = NULL;

CTutorial *CManager::m_pTutorial = NULL;

CGame *CManager::m_pGame = NULL;

CResult *CManager::m_pResult = NULL;

CSound *CManager::m_pSound = NULL;

CPause *CManager::m_pPause = NULL;
bool CManager::m_bPause = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_pRenderer = new CRenderer;

	// レンダラ初期化
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	m_pKey = new CKey;

	// キーボード初期化
	if (FAILED(m_pKey->Init(hInstance, hWnd)))
	{
		return -1;
	}
	m_pSound = new CSound;

	if (FAILED(m_pSound->Init(hWnd)))
	{
		return -1;
	}

	m_pFade = new CFade;

	//フェードクラスのクリエイト
	m_pFade->Init();

	// フェードしてからタイトルへ
	m_pFade->SetFade(MODE_GAME);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//現在モードの終了
	UninitMode(m_mode);

	if (m_pFade != NULL)		// フェード
	{// NULLチェック
		m_pFade->Uninit();		// 終了処理
		delete m_pFade;			// メモリの破棄
		m_pFade = NULL;			// メモリのクリア
	}
	if (m_pRenderer != NULL)	// レンダラー
	{// NULLチェック
		m_pRenderer->Uninit();	// 終了処理
		delete m_pRenderer;		// メモリの破棄
		m_pRenderer = NULL;		// メモリのクリア
	}
	if (m_pKey != NULL)			// キー入力
	{// NULLチェック
		m_pKey->Uninit();		// 終了処理
		delete m_pKey;			// メモリの破棄
		m_pKey = NULL;			// メモリのクリア
	}
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// シーンの全削除
	CScene::ReleaseAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();	// レンダラークラスの更新処理
	}
	if (m_pKey != NULL)
	{
		m_pKey->Update();		// キーボードクラスの更新処理
	}
	if (m_pFade != NULL)
	{
		m_pFade->Update();		// フェードクラスの更新処理
	}

	PauseUpdate();
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();	// 描画処理
	}
}

//=============================================================================
// モードの終了処理
//=============================================================================
void CManager::UninitMode(MODE mode)
{
	// モードの終了
	switch (mode)
	{
	case MODE_TITLE:// タイトル
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();// 終了処理
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:// チュートリアル
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();// 終了処理
			m_pTutorial = NULL;
		}
		break;
	case MODE_GAME:// ゲーム
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();// 終了処理
			m_pGame = NULL;
		}
		break;
	case MODE_RESULT:// リザルト
		if (m_pResult != NULL)
		{
			m_pResult->Uninit();// 終了処理
			m_pResult = NULL;
		}
		break;
	}
}

//=============================================================================
// モードの生成
//=============================================================================
void CManager::CreateMode(MODE mode)
{
	//設定されたモードをクリエイト
	switch (mode)
	{
	case MODE_TITLE:	// タイトル
		if (m_pTitle == NULL)
		{
			m_pTitle = CTitle::Create();	
		}
		break;
	case MODE_TUTORIAL:	// チュートリアル
		if (m_pTutorial == NULL)
		{
			m_pTutorial = CTutorial::Create();	
		}
		break;
	case MODE_GAME:		// ゲーム
		if (m_pGame == NULL)
		{
			m_pGame = CGame::Create();			
		}
		break;
	case MODE_RESULT:	// リザルト
		if (m_pGame == NULL)
		{
			m_pResult = CResult::Create();		
		}
	default:
		break;
	}
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(MODE mode)
{
	//現在モードの終了
	UninitMode(m_mode);

	// シーン情報のリリース
	CScene::ReleaseAll();

	//モードを設定
	m_mode = mode;
	m_bPause = false;
	m_pPause = NULL;

	// モードの生成
	CreateMode(m_mode);
}

//=============================================================================
// モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// レンダラークラスのゲット関数
//=============================================================================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// キーボード入力取得
//=============================================================================
CKey* CManager::GetKey(void)
{
	return m_pKey;
}

void CManager::PauseUpdate(void)
{
	CKey *pKey = CManager::GetKey();	   // キー入力情報
	CFade::FADE Fade = CFade::GetFade();   // フェード情報

	if (Fade == CFade::FADE_NONE && m_mode == MODE_GAME)
	{// フェードが何もしていない時
		if (pKey->GetState(CKey::STATE_TRIGGER, DIK_P) == true)
		{// Pが押されたとき
			m_bPause = m_bPause ? false : true;	// falseかtrueに切り替える
		}
		if (m_bPause == true && m_pPause == NULL)
		{// ポーズが開始されたとき
			m_pPause = CPause::Create();	// ポーズを生成
		}
		else if (m_bPause == false && m_pPause != NULL)
		{
			m_pPause->Uninit();
			m_pPause = NULL;
		}
	}
}

