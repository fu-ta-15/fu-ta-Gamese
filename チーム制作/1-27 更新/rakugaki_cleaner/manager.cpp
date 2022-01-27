//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : 
//
//=============================================================================
#include "manager.h"

#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "input_mouse.h"
#include "sound.h"
#include "fade.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "texture.h"
#include "base.h"
#include "keep_object.h"
#include "shadow.h"

#include "title.h" 
#include "game.h"
#include "result.h"
#include "mode_select.h"
#include "preparation_player.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CInputMouse *CManager::m_pInputMouse = NULL;
CSound *CManager::m_pSound = NULL;
CBase *CManager::m_pBase = NULL;					
CManager::MODE CManager::m_mode = MODE_TITLE;
CFade *CManager::m_pFade = NULL;
bool CManager::m_bPause = false;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
CTexture *CManager::m_pTexture = NULL;
CKeepObject *CManager::m_pKeepObject = NULL;

//=============================================================================
// CManagerのコンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// CManagerのデストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, bool bWindow)
{
	// 現在時刻によってランダムに設定する
	srand((unsigned)time(NULL));

	// レンダラーの初期化処理
	m_pRenderer = new CRenderer;

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, bWindow);
	}

	// キーボードの初期化処理
	m_pInputKeyboard = new CInputKeyboard;

	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	// ジョイパッド情報の初期化処理
	m_pInputJoypad = new CInputJoypad;

	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Init(hInstance, hWnd);
	}

	// マウス情報の初期化処理
	m_pInputMouse = new CInputMouse;

	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Init(hInstance, hWnd);
	}

	// サウンドの初期化処理
	m_pSound = new CSound;

	if (m_pSound != NULL)
	{
		m_pSound->Init(hWnd);
	}

	// テクスチャ管理の生成
	m_pTexture = new CTexture;

	if (m_pTexture != NULL)
	{
		m_pTexture->Load();
	} 

	// モードの確定
	CBase *pBase = SetMode();

	// フェードの初期化処理
	m_pFade = new CFade;

	if (m_pFade != NULL)
	{
		m_pFade->Init(pBase);
	}

	// ライトの初期化
	m_pLight = new CLight;

	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

	// デバック表示の生成
	CDebugProc *pDebugProc = new CDebugProc;

	if (pDebugProc != NULL)
	{
		pDebugProc->Init();
	}

	// データ保存用クラスの生成
	m_pKeepObject = new CKeepObject;

	if (m_pKeepObject != NULL)
	{
		m_pKeepObject->Init();
	}

	// モードの呼び出し
	SetMode(pBase);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// サウンドの終了処理
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// フェードの終了処理
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// レンダラの終了処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// オブジェクトの終了処理
	CScene::ReleaseAll();

	// ベースの破棄
	if (m_pBase = NULL)
	{
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = NULL;
	}

	// テクスチャのアンロード
	if (m_pTexture != NULL)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	// マウスの終了処理
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// ジョイパッドの終了処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// キーボードの終了処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// ジョイパッドの更新処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	// マウスの更新処理
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}

	// レンダラの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// ベースの更新処理
	if (m_pBase != NULL)
	{
		m_pBase->Update();
	}

	// オブジェクトの更新処理
	CScene::UpdateAll();

	// フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	if (m_pKeepObject != NULL)
	{
		m_pKeepObject->Update();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// レンダラの描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モードの設定
//=============================================================================
void CManager::SetMode(CBase *pBase)
{
	// オブジェクトの削除
	CScene::ReleaseAll();

	// ベースの破棄
	if (m_pBase != NULL)
	{
		m_pBase->Uninit();
		delete m_pBase;
		m_pBase = NULL;
	}

	// モードの移行
	m_pBase = pBase;

	//シーンの初期化
	if (m_pBase != NULL)
	{
		m_pBase->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	}
}

//=============================================================================
// モードの確定
//=============================================================================
CBase *CManager::SetMode(void)
{
	CBase *pBase = NULL;

	// モードの確定
	switch (m_mode)
	{
	case MODE_TITLE:
		pBase = new CTitle;
		break;

	case MODE_MODESELECT:
		pBase = new CModeSelect;
		break;

	case MODE_PREPARATION:
		pBase = new CPreparationPlayer;
		break;

	case MODE_GAME:
		pBase = new CGame;
		break;

	case MODE_RESULT:
		pBase = new CResult;
		break;
	}

	return pBase;
}