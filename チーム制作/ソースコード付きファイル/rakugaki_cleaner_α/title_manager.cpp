//=============================================================================
//
// タイトル画面管理処理 [title_manager.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "title_manager.h"
#include "manager.h"
#include "base.h"
#include "title.h"
#include "press_logo.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "debugproc.h"

//=============================================================================
// CTitleManagerのコンストラクタ
//=============================================================================
CTitleManager::CTitleManager()
{
	// メンバ変数の初期化
	m_pPressLogo = NULL;
	m_bPress = false;
}

//=============================================================================
// CTitleManagerのデストラクタ
//=============================================================================
CTitleManager::~CTitleManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitleManager::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_pPressLogo = NULL;
	m_bPress = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitleManager::Uninit(void)
{
	// 削除
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitleManager::Update(void)
{
	// キー情報とパッド情報を取得
	CInputKeyboard *pKey = GET_KEYBOARD;
	CInputJoypad *pJoypad = GET_JOYPAD;

	// ボタンを押したらtrueにする
	if (m_bPress == false)
	{
		if (pKey->GetTrigger(KEY_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_DECISION))
		{
			m_pPressLogo->PressButton();

			// trueにする
			m_bPress = true;
		}
	}
	else
	{
		// 遷移できるとき処理
		if (m_pPressLogo->GetTransition())
		{
			// 遷移先を指定
			CTitle *pTitle = (CTitle*)(CManager::GetBase());

			pTitle->SetTitleMode(CTitle::TITLEMODE_TRANSITION);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitleManager::Draw(void)
{

}