//*****************************************************************************
//
// キーボード入力処理 [input.hp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "keyinput.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CKey::CKey()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CKey::~CKey()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CKey::Init(HINSTANCE hInstance, HWND hWnd)
{
	//入力処理クラスの初期化処理呼び出し
	CInput::Init(hInstance, hWnd);

	if (FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDDevice, NULL)))
	{// 入力デバイスの生成
		return E_FAIL;
	}
	if (FAILED(m_pDDevice->SetDataFormat(&c_dfDIKeyboard)))
	{// データフォーマットの設定
		return E_FAIL;
	}
	if (FAILED(m_pDDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{// 協調モードの設定
		return E_FAIL;
	}

	// アクセス権の取得
	m_pDDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CKey::Uninit(void)
{
	//入力処理クラスの終了処理呼び出し
	CInput::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CKey::Update(void)
{
	BYTE aKeyState[KEY_MAX];

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nKey = 0; nKey < KEY_MAX; nKey++)
		{
			m_aKeyState[STATE_PRESSE] [nKey]	=	 aKeyState[nKey];											// プレス

			m_aKeyState[STATE_TRIGGER][nKey]	=	 (~m_aKeyState[STATE_NONE][nKey])& (aKeyState[nKey]);		// トリガー

			m_aKeyState[STATE_RELEASE][nKey]	=	 (m_aKeyState[STATE_NONE][nKey]) & (~aKeyState[nKey]);		// リリース

			m_aKeyState[STATE_NONE]   [nKey]	=	 aKeyState[nKey];
		}
	}
	else
	{// アクセス権の取得
		m_pDDevice->Acquire();
	}
}

//=============================================================================
// 入力判定		[ 状態・キーナンバー ]
//=============================================================================
bool CKey::GetState(STATE state, int nKey)
{
	return (m_aKeyState[state][nKey] & 0x80) ? true : false;
}
