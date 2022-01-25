//=============================================================================
//
// ジョイパッド処理 [input_joypad.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DATA		(256)													// 最大データ数
#define MAX_CONTROLLER	(4)														// 接続可能なコントローラーの最大数

#define JOYPAD_DECISION				(CInputJoypad::JPINFO_A)					// 決定ボタン
#define JOYPAD_CANCEL				(CInputJoypad::JPINFO_B)					// 決定ボタン

#define JOYPAD_JUMP					(CInputJoypad::JPINFO_A)					// ジャンプボタン
#define JOYPAD_POINT_ROT_RIGHT		(CInputJoypad::JPINFO_RB)					// ジャンプボタン
#define JOYPAD_POINT_ROT_LEFT		(CInputJoypad::JPINFO_LB)					// ジャンプボタン

#define JOYPAD_GRAFFITI_DELETE		(CInputJoypad::JPINFO_RB)					// ラクガキ削除
#define JOYPAD_ERASERTYPE_SWITCH	(CInputJoypad::JPINFO_X)					// 消しゴム切り替え
#define JOYPAD_SKILL				(CInputJoypad::JPINFO_A)					// スキル

#define JOYPAD_UP					(CInputJoypad::STICK_AND_CROSSKEY_UP)		// 上
#define JOYPAD_DOWN					(CInputJoypad::STICK_AND_CROSSKEY_DOWN)		// 下
#define JOYPAD_RIGHT				(CInputJoypad::STICK_AND_CROSSKEY_RIGHT)	// 右
#define JOYPAD_LEFT					(CInputJoypad::STICK_AND_CROSSKEY_LEFT)		// 左

#define JOYPAD_PAUSE				(CInputJoypad::JPINFO_START)				// ポーズ

//*****************************************************************************
// ジョイパッドクラス(派生クラス)
//*****************************************************************************
class CInputJoypad : public CInput
{
public:
	typedef enum
	{
		JPINFO_X = 0,	// X
		JPINFO_Y,		// Y
		JPINFO_A,		// A
		JPINFO_B,		// B
		JPINFO_LB,		// LB
		JPINFO_RB,		// RB
		JPINFO_LT,		// LT
		JPINFO_RT,		// RT
		JPINFO_LS,		// LS
		JPINFO_RS,		// RS
		JPINFO_BACK,	// BACK
		JPINFO_START,	// START
		JPINFO_GUIDE	// GUIDE
	}JOYPADINFO;

	typedef enum
	{
		STICK_AND_CROSSKEY_UP = 0,	// 上
		STICK_AND_CROSSKEY_DOWN,	// 下
		STICK_AND_CROSSKEY_RIGHT,	// 右
		STICK_AND_CROSSKEY_LEFT,	// 左
		STICK_AND_CROSSKEY_MAX,
	}StickAndCrossKey;

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	void LStickPressInfo(int nCnt, DIJOYSTATE2 State);											// Lスティックのプレス処理
	void RStickPressInfo(int nCnt, DIJOYSTATE2 State);											// Rスティックのプレス処理
	void StickTriggerInfo(int nCnt, DIJOYSTATE2 State);											// スティックのトリガー処理

	void CrossKeyPressInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのプレス処理
	void CrossKeyTriggerInfo(int nCnt, DIJOYSTATE2 State);										// ボタンのトリガー処理

	bool GetButtonPress(int nCnt, int nButton);													// ボタンのプレス情報の取得
	bool GetButtonTrigger(int nCnt, int nButton);												// ボタンのトリガー情報の取得
	bool GetButtonReleace(int nCnt, int nButton);												// ボタンのリリース情報の取得

	DIJOYSTATE2 GetControllerPless(int nCnt);													// コントローラーのプレス情報の取得
	bool *GetLStickPress(int nCnt);																// Lスティックのプレス情報の取得
	bool *GetRStickPress(int nCnt);																// Rスティックのプレス情報の取得
	bool *GetStickTrigger(int nCnt);															// スティックのトリガー情報の取得

	bool *GetCrossKeyPress(int nCnt);															// ボタンのプレス情報の取得
	bool *GetCrossKeyTrigger(int nCnt);															// ボタンのトリガー情報の取得

	int GetnCntController(void);

private:
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// コールバック関数

	typedef struct 
	{
		LPDIRECTINPUTDEVICE8 pDevjoypad = NULL;	// ジョイパッドのデバイス情報
		DIJOYSTATE2 State;						// プレス
		DIJOYSTATE2 Trigger;					// トリガー
		DIJOYSTATE2 Release;					// リリース
	}CONTROLLER;								// コントローラーの情報							

	typedef struct 
	{
		bool bLStickPress[STICK_AND_CROSSKEY_MAX];			// Lスティックのプレス情報
		bool bRStickPress[STICK_AND_CROSSKEY_MAX];			// Rスティックのプレス情報
		bool bStickTrigger[STICK_AND_CROSSKEY_MAX];			// スティックのトリガー情報
		bool bStickOldTrigger[STICK_AND_CROSSKEY_MAX];		// スティックのトリガー情報を保存
		bool bCrossKeyPress[STICK_AND_CROSSKEY_MAX];		// 十字キーのプレス情報
		bool bCrossKeyTrigger[STICK_AND_CROSSKEY_MAX];		// 十字キーのトリガー情報
		bool bCrossKeyOldTrigger[STICK_AND_CROSSKEY_MAX];	// 十字キーのトリガー情報を保存
	}JPINFO;												// コントローラーの操作情報										

	static CONTROLLER m_Controller[MAX_CONTROLLER];			// コントローラー情報のポインタ
	static JPINFO m_Stick[MAX_CONTROLLER];					// スティック情報
	static int m_nNumController;							// 使用するコントローラーのカウント
};
#endif