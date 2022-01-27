//=============================================================================
//
// キーボード処理 [input_keyboard.h]
// Author : 
//
//=============================================================================
#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_

#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DATA							(256)										// 最大データ数
#define KEY_LookAtPoint_LEFT				(CInputKeyboard::KEYINFO_LookAtPoint_LEFT)	// 注視点左回転
#define KEY_LookAtPoint_RIGHT				(CInputKeyboard::KEYINFO_LookAtPoint_RIGHT)	// 注視点右回転
#define KEY_Point_LEFT						(CInputKeyboard::KEYINFO_Point_LEFT)		// 視点左回転
#define KEY_Point_RIGHT						(CInputKeyboard::KEYINFO_Point_RIGHT)		// 視点右回転
#define KEY_MOVE_UP							(CInputKeyboard::KEYINFO_UP)				// 上移動
#define KEY_MOVE_DOWN						(CInputKeyboard::KEYINFO_DOWN)				// 下移動
#define KEY_MOVE_RIGHT						(CInputKeyboard::KEYINFO_RIGHT)				// 右移動
#define KEY_MOVE_LEFT						(CInputKeyboard::KEYINFO_LEFT)				// 左移動
#define KEY_MOVE_LookAtPoint_UP				(CInputKeyboard::KEYINFO_LookAtPoint_UP)	// 注視点上移動
#define KEY_MOVE_LookAtPoint_DOWN			(CInputKeyboard::KEYINFO_LookAtPoint_DOWN)	// 注視点下移動
#define KEY_MOVE_Point_UP					(CInputKeyboard::KEYINFO_Point_UP)			// 視点上移動
#define KEY_MOVE_Point_DOWN					(CInputKeyboard::KEYINFO_Point_DOWN)		// 視点下移動
#define KEY_FLAME							(CInputKeyboard::KEYINFO_FLAME)				// 視点下移動
#define KEY_MOVE_Up							(CInputKeyboard::KEYINFO_Up)				// 上移動
#define KEY_MOVE_Down						(CInputKeyboard::KEYINFO_Down)				// 下移動
#define KEY_MOVE_Right						(CInputKeyboard::KEYINFO_Right)				// 右移動
#define KEY_MOVE_Left						(CInputKeyboard::KEYINFO_Left)				// 左移動

#define KEY_RESET							(CInputKeyboard::KEYINFO_RESET)				// 左移動

#define KEY_PAUSE							(CInputKeyboard::KEYINFO_PAUSE)				// 左移動
#define KEY_OK								(CInputKeyboard::KEYINFO_OK)				// 左移動

#define KEY_PLAYEROP_UP						(CInputKeyboard::KEYINFO_PLAYEROP_UP)		// 左移動
#define KEY_PLAYEROP_DOWN					(CInputKeyboard::KEYINFO_PLAYEROP_DOWN)		// 左移動

#define KEY_DELETE							(CInputKeyboard::KEYINFO_DELETE)		// 左移動
#define KEY_SWITCH_ERASER					(CInputKeyboard::KEYINFO_SWITCH_ERASER)		// 左移動
#define KEY_SKILL							(CInputKeyboard::KEYINFO_SKILL)		// 左移動

//-----------------------------------------------------------------------------
// キーボードクラス(派生クラス)
//-----------------------------------------------------------------------------
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	// キー入力情報
	typedef enum
	{
		KEYINFO_OK = 0,				// 決定
		KEYINFO_UP,					// 上選択
		KEYINFO_DOWN,				// 下選択
		KEYINFO_RIGHT,				// 右移動
		KEYINFO_LEFT,				// 左移動
		KEYINFO_PAUSE,				// ポーズ
		KEYINFO_LookAtPoint_LEFT,	// 注視点左回転
		KEYINFO_LookAtPoint_RIGHT,	// 注視点右回転
		KEYINFO_Point_LEFT,			// 視点左回転
		KEYINFO_Point_RIGHT,		// 視点右回転
		KEYINFO_LookAtPoint_UP,		// 注視点上移動
		KEYINFO_LookAtPoint_DOWN,	// 注視点下移動
		KEYINFO_Point_UP,			// 視点上移動
		KEYINFO_Point_DOWN,			// 視点下移動
		KEYINFO_FLAME,				// ワイヤーフレームの表示
		KEYINFO_Up,
		KEYINFO_Down,
		KEYINFO_Right,
		KEYINFO_Left,
		KEYINFO_BuffReset,
		KEYINFO_RESET,
		KEYINFO_PLAYEROP_DOWN,
		KEYINFO_PLAYEROP_UP,
		KEYINFO_DELETE,
		KEYINFO_SWITCH_ERASER,
		KEYINFO_SKILL,
		KEYINFO_MAX,
	}KEYINFO;

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetReleace(int nKey);

private:
	BYTE m_aKeyState[MAX_DATA];
	BYTE m_aKeyStateTrigger[MAX_DATA];
	BYTE m_aKeyStateReleace[MAX_DATA];
	BYTE m_aKeyStateRepeat[MAX_DATA];

	int m_aKeyInfo[KEYINFO_MAX] =
	{
		DIK_RETURN,		// 決定
		DIK_W,			// 上選択
		DIK_S,			// 下選択 
		DIK_D,			// 右移動
		DIK_A,			// 左移動
		DIK_P,			// ポーズ
		DIK_Q,			// 注視点左回転
		DIK_E,			// 注視点右回転
		DIK_Z,			// 視点左回転
		DIK_C,			// 視点右回転
		DIK_T,			// 注視点上移動
		DIK_B,			// 注視点下移動
		DIK_R,			// 視点上移動
		DIK_V,			// 視点下移動
		DIK_M,			// ワイヤーフレームの表示
		DIK_UP,
		DIK_DOWN,
		DIK_RIGHT,
		DIK_LEFT,
		DIK_F2,
		DIK_LCONTROL,
		DIK_9,			// 操作可能プレイヤーの選択
		DIK_0,			// 操作可能プレイヤーの選択
		DIK_SPACE,		// 消す動き
		DIK_K,			// 切り替え
		DIK_J,			// スキル
	};
};
#endif