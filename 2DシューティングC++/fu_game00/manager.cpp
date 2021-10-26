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
CRenderer *CManager::m_pRenderer = NULL;				  // レンダラーポインタ
CKey *CManager::m_pKey = NULL;							  // キーボード入力ポインタ
CFade *CManager::m_pFade = NULL;						  // フェードポインタ
CTitle *CManager::m_pTitle = NULL;						  // タイトルポインタ
CTutorial *CManager::m_pTutorial = NULL;				  // チュートリアルポインタ
CGame *CManager::m_pGame = NULL;						  // ゲームポインタ
CResult *CManager::m_pResult = NULL;					  // リザルトポインタ
CSound *CManager::m_pSound = NULL;						  // サウンドポインタ
CPause *CManager::m_pPause = NULL;						  // ポーズポインタ
bool CManager::m_bPause = false;						  // ポーズの切り替え変数
CManager::MODE CManager::m_mode = CManager::MODE_TITLE;	  // 現在のモードの変数

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
	// 現在時刻の情報で初期化
	srand((unsigned)time(NULL));	

	// レンダラー生成
	m_pRenderer = new CRenderer;

	// レンダラ初期化
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return -1;
	}

	// キーボード生成
	m_pKey = new CKey;

	// キーボード初期化
	if (FAILED(m_pKey->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// サウンドの生成
	m_pSound = new CSound;

	// サウンドの初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return -1;
	}

	// フェードの生成
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
		// タイトルモード
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{// NULLチェック
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}
		break;

		// チュートリアルモード
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// NULLチェック
			m_pTutorial->Uninit();
			m_pTutorial = NULL;
		}
		break;

		// ゲームモード
	case MODE_GAME:
		if (m_pGame != NULL)
		{// NULLチェック
			m_pGame->Uninit();
			m_pGame = NULL;
		}
		break;

		// リザルトモード
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
		// タイトルモード
	case MODE_TITLE:	
		if (m_pTitle == NULL)
		{// NULLチェック
			m_pTitle = CTitle::Create();
		}
		break;

		// チュートリアルモード
	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// NULLチェック
			m_pTutorial = CTutorial::Create();
		}
		break;

		// ゲームモード
	case MODE_GAME:		
		if (m_pGame == NULL)
		{// NULLチェック
			m_pGame = CGame::Create();
		}
		break;

		// リザルトモード
	case MODE_RESULT:	
		if (m_pResult == NULL)
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

	// ポーズの状態を初期化
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
	// キー入力情報
	CKey *pKey = CManager::GetKey();	   

	// フェード情報
	CFade::FADE Fade = CFade::GetFade();  

	// フェードが何もしていない時
	if (Fade == CFade::FADE_NONE && m_mode == MODE_GAME)
	{
		// Pが押されたとき
		if (pKey->GetState(CKey::STATE_TRIGGER, DIK_P) == true)
		{
			// falseかtrueに切り替える
			m_bPause = m_bPause ? false : true;	
		}

		// ポーズが開始されたとき
		if (m_bPause == true && m_pPause == NULL)
		{
			// ポーズを生成
			m_pPause = CPause::Create();	
		}
		else if (m_bPause == false && m_pPause != NULL)
		{
			// ポーズの開放
			m_pPause->Uninit();

			// NULLを代入
			m_pPause = NULL;
		}
	}
}

