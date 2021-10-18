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
	srand((unsigned)time(NULL));	// 現在時刻の情報で初期化

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

	// サウンドの初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return -1;
	}

	m_pFade = new CFade;

	//フェードの初期化
	m_pFade->Init();

	// フェードしてからタイトルへ
	m_pFade->SetFade(MODE_TITLE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//現在モードの終了
	UninitMode(m_mode);

	// フェードの開放
	if (m_pFade != NULL)		
	{// NULLチェック
		m_pFade->Uninit();		
		delete m_pFade;			
		m_pFade = NULL;			
	}

	// レンダラーの開放
	if (m_pRenderer != NULL)	
	{// NULLチェック
		m_pRenderer->Uninit();	
		delete m_pRenderer;		
		m_pRenderer = NULL;		
	}

	// キーの開放
	if (m_pKey != NULL)			
	{// NULLチェック
		m_pKey->Uninit();		
		delete m_pKey;			
		m_pKey = NULL;			
	}

	// サウンドの開放
	if (m_pSound != NULL)
	{// NULLチェック
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
	// レンダラーの更新
	if (m_pRenderer != NULL)
	{// NULLチェック
		m_pRenderer->Update();	
	}

	// キーの更新
	if (m_pKey != NULL)
	{// NULLチェック
		m_pKey->Update();		
	}

	// フェードの更新
	if (m_pFade != NULL)
	{// NULLチェック
		m_pFade->Update();		
	}

	// ポーズの更新
	PauseUpdate();
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// 描画処理
	if (m_pRenderer != NULL)
	{// NULLチェック
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モードの終了処理
//=============================================================================
void CManager::UninitMode(MODE mode)
{
	// 各モードの開放
	switch (mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// NULLチェック
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// NULLチェック
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{// NULLチェック
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{// NULLチェック
			m_pResult->Uninit();
			m_pResult = NULL;
		}
		break;
	default:
		break;
	}
}

//=============================================================================
// モードの生成
//=============================================================================
void CManager::CreateMode(MODE mode)
{
	// 各モードの生成
	switch (mode)
	{
	case MODE_TITLE:	
		if (m_pTitle == NULL)
		{// NULLチェック
			m_pTitle = CTitle::Create();
		}
		break;

	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// NULLチェック
			m_pTutorial = CTutorial::Create();
		}
		break;

	case MODE_GAME:		
		if (m_pGame == NULL)
		{// NULLチェック
			m_pGame = CGame::Create();
		}
		break;

	case MODE_RESULT:	
		if (m_pGame == NULL)
		{// NULLチェック
			m_pResult = CResult::Create();
		}
		break;
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

//=============================================================================
// ポーズの更新
//=============================================================================
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

