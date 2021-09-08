//##################################################################################################################################################################//
//
// パッド入力ヘッダファイル [padinput.h]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//
#ifndef _PADINPUT_H_
#define _PADINPUT_H_

//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "input.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CDInput : public CInput
{
public:
	//-----------------------------------------------------------------------------
	// 入力の状態
	//-----------------------------------------------------------------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_PRESSE,
		STATE_TRIGGER,
		STATE_RELEASE,
		STATE_MAX
	}STATE;

	CDInput();
	~CDInput();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

private:

};



typedef enum
{
	JPPRESS_RIGTH = 0,
	JPPRESS_LEFT,
	JPPRESS_UP,
	JPPRESS_DOWN,
	JPPRESS_MAX
}JPPRESS;

typedef enum
{
	JPTRIGGER_RIGTH = 0,
	JPTRIGGER_LEFT,
	JPTRIGGER_UP,
	JPTRIGGER_DOWN,
	JPTRIGGER_MAX
}JPTRIGGER;

typedef enum
{
	D_BUUTON_X = 0,		// 1
	D_BUUTON_Y,			// 2
	D_BUUTON_A,			// 3
	D_BUUTON_B,			// 4
	D_BUUTON_LB,		// 5
	D_BUUTON_RB,		// 6
	D_BUUTON_LT,		// 7
	D_BUUTON_RT,		// 8
	D_BUUTON_LEFT,		// 9
	D_BUUTON_RIGHT,		// 10
	D_BUUTON_BACK,		// 11
	D_BUUTON_START,		// 12
	D_BUUTON_GUIDE,		// 13
	D_BUUTON_MAX
}D_BUUTON;

typedef enum
{
	D_CROSS_UP = 0, 
	D_CROSS_DOWN,
	D_CROSS_RIGHT,
	D_CROSS_LEFT,
	D_CROSS_MAX
}D_CROSS_BUUTON;

//=====================================================================================================================================================================//
// プロトタイプ宣言
//=====================================================================================================================================================================//
HRESULT InitKeypad(HINSTANCE hInstance, HWND hWnd);
void UninitKeypad(void);
void UpdateKeypad(void);
bool GetKeypadTrigger(int nKey);
bool GetKeypadPresse(int nKey);
bool GetKeypadReleace(int nKey);
void JoypadPressInfo(DIJOYSTATE2 State);
void JoypadTriggerInfo(DIJOYSTATE2 State);
bool *GetPadPlress(void);
bool *GetPadTrigger(void);
void CrossKeyPressInfo(DIJOYSTATE2 State);
void CrossKeyTriggerInfo(DIJOYSTATE2 State);


DIJOYSTATE2 *pGetPadCont(void);

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);

#endif