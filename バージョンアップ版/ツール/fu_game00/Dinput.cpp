//*****************************************************************************
//
// ゲームパッド入力処理 [input.hp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#include "Dinput.h"

//-------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------
#define DEADZONE        2500            // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       3000           // 有効範囲の最大値
#define RANGE_MIN       -3000          // 有効範囲の最小値

//-------------------------------------------------------------------
// 構造体の定義
//-------------------------------------------------------------------
typedef struct
{
	LPDIRECTINPUTDEVICE8 g_lpDIDevice = NULL;	// ジョイパッドのデバイス情報
	DIJOYSTATE2 State;						// プレス
	DIJOYSTATE2 Trigger;					// トリガー
	DIJOYSTATE2 Release;					// リリース
}CONTROLLER;								// コントローラーの情報


//-------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------
LPDIRECTINPUT8 g_lpDI = NULL;				// Dinputのポインタ
CONTROLLER g_Controller;					// コントローラー情報のポインタ
bool g_bPress[JPPRESS_MAX];					// プレス情報
bool g_bTrigger[JPTRIGGER_MAX];				// スティックを倒したか
bool g_bStick[JPTRIGGER_MAX];


//=====================================================================================================================================================================//
//初期化処理
//=====================================================================================================================================================================//
HRESULT InitKeypad(HINSTANCE hInstance, HWND hWnd)
{
	// 初期化
	for (int nCuntPress = 0; nCuntPress < JPPRESS_MAX; nCuntPress++)
	{
		g_bPress[nCuntPress] = false;
	}

	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_lpDI, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイスの生成
	if (FAILED(g_lpDI->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// コントローラー情報の初期化
	if (!g_Controller.g_lpDIDevice)
	{
		return E_FAIL;
	}

	// ボタンの初期化
	for (int nCntButton = 0; nCntButton < 13; nCntButton++)
	{
		g_Controller.State.rgbButtons[nCntButton] = '\0';
		g_Controller.Trigger.rgbButtons[nCntButton] = '\0';
		g_Controller.Release.rgbButtons[nCntButton] = '\0';
	}

	// 有効値チェック
	if (g_Controller.State.rgdwPOV[0] != 0xFFFFFFFF)
	{
		// 有効
	}

	//データフォーマットの設定
	if (FAILED(g_Controller.g_lpDIDevice->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}

	//協調モードの設定
	if (FAILED(g_Controller.g_lpDIDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// 左スティックの初期化
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	// X軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_X;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Y軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Y;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z軸の範囲を設定
	diprg.diph.dwObj = DIJOFS_Z;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
	// Z回転の範囲を設定
	diprg.diph.dwObj = DIJOFS_RZ;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	// 右スティックの初期化
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	// X軸の範囲を設定
	dipdw.diph.dwObj = DIJOFS_X;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Y軸の範囲を設定
	dipdw.diph.dwObj = DIJOFS_Y;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Z軸の範囲を設定
	dipdw.diph.dwObj = DIJOFS_Z;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
	// Z回転の範囲を設定
	dipdw.diph.dwObj = DIJOFS_RZ;
	g_Controller.g_lpDIDevice->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

	//アクセス権の取得
	g_Controller.g_lpDIDevice->Acquire();

	return S_OK;
}

//=====================================================================================================================================================================//
//終了処理
//=====================================================================================================================================================================//
void UninitKeypad(void)
{
	//データフォーマットの開放
	if (g_Controller.g_lpDIDevice != NULL)
	{
		//アクセス権の破棄
		g_Controller.g_lpDIDevice->Unacquire();
		g_Controller.g_lpDIDevice->Release();
		g_Controller.g_lpDIDevice = NULL;
	}

	//DirectInputオブジェクトの開放
	if (g_lpDI != NULL)
	{
		g_lpDI->Release();
		g_lpDI = NULL;
	}
}

//=====================================================================================================================================================================//
//更新処理
//=====================================================================================================================================================================//
void UpdateKeypad(void)
{
	// 変数宣言
	DIJOYSTATE2 ControllerState;	// コントローラーのボタン情報
	HRESULT hr;						// 成功したか判別
	int nCntButton;					// ボタンの個数

	if (g_Controller.g_lpDIDevice != NULL)
	{// ジョイパッドが接続されていたらポーリング開始
		hr = g_Controller.g_lpDIDevice->Poll();

		// デバイスからデータを取得
		if (SUCCEEDED(hr = g_Controller.g_lpDIDevice->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
		{// スティックに情報を代入
			JoypadPressInfo(ControllerState);			// プレス
			JoypadTriggerInfo(ControllerState);			// トリガー

			// 左スティックの情報
			g_Controller.State.lY = ControllerState.lY;			// lYのプレス
			g_Controller.State.lX = ControllerState.lX;			// lXのプレス
			g_Controller.State.lZ = ControllerState.lZ;			// lZのプレス
			g_Controller.State.lRz = ControllerState.lRz;		// lRzのプレス

			CrossKeyPressInfo(ControllerState);

			// プレス情報の保存
			g_Controller.State.rgdwPOV[0] = ControllerState.rgdwPOV[0];

			for (nCntButton = 0; nCntButton < 13; nCntButton++)
			{
				// トリガー情報の保存
				g_Controller.Trigger.rgbButtons[nCntButton] =
					(g_Controller.State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

				// リリース情報の保存
				g_Controller.Release.rgbButtons[nCntButton] =
					(ControllerState.rgbButtons[nCntButton] ^ g_Controller.State.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

				// プレス情報の保存
				g_Controller.State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];
			}
		}
		else
		{// アクセス権の取得
			g_Controller.g_lpDIDevice->Acquire();
		}
	}
}

//=====================================================================================================================================================================//
// ボタン情報の取得
//=====================================================================================================================================================================//
bool GetKeypadTrigger(int nKey)
{// トリガー情報の確認
	return (g_Controller.Trigger.rgbButtons[nKey] & 0x80) ? true : false;
}
bool GetKeypadPresse(int nKey)
{// プレス情報の確認
	return (g_Controller.State.rgbButtons[nKey] & 0x80) ? true : false;
}
bool GetKeypadReleace(int nKey)
{// リリース情報の確認
	return (g_Controller.Release.rgbButtons[nKey] & 0x80) ? true : false;
}


//=====================================================================================================================================================================//
// コールバック関数
//=====================================================================================================================================================================//
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	hr = g_lpDI->CreateDevice(pdidInstance->guidInstance, &g_Controller.g_lpDIDevice, NULL);

	return DIENUM_CONTINUE;
}

//=====================================================================================================================================================================//
// スティック情報の取得
//=====================================================================================================================================================================//
void JoypadPressInfo(DIJOYSTATE2 State)
{// プレス
	bool bPress[JPPRESS_MAX] = { false,false,false,false };

	if (State.lX >= 3000)
	{// 右
		bPress[JPPRESS_RIGTH] = true;
	}
	else if (State.lX <= -3000)
	{// 左
		bPress[JPPRESS_LEFT] = true;
	}
	else if (State.lY >= 3000)
	{// 下
		bPress[JPPRESS_DOWN] = true;
	}
	else if (State.lY <= -3000)
	{// 上
		bPress[JPPRESS_UP] = true;
	}

	// 情報
	for (int nCuntPress = 0; nCuntPress < JPPRESS_MAX; nCuntPress++)
	{
		g_bPress[nCuntPress] = bPress[nCuntPress];
	}
}
void JoypadTriggerInfo(DIJOYSTATE2 State)
{// トリガー
	bool bTrigger[JPTRIGGER_MAX] = { false,false,false,false };

	if (State.lX >= 3000)
	{// 右
		bTrigger[JPTRIGGER_RIGTH] = true;
	}
	else if (State.lX <= -3000)
	{// 左
		bTrigger[JPTRIGGER_LEFT] = true;
	}
	else if (State.lY >= 3000)
	{// 下
		bTrigger[JPPRESS_DOWN] = true;
	}
	else if (State.lY <= -3000)
	{// 上
		bTrigger[JPTRIGGER_UP] = true;
	}

	// 情報
	for (int nCuntTrigger = 0; nCuntTrigger < JPPRESS_MAX; nCuntTrigger++)
	{
		g_bTrigger[nCuntTrigger] = g_bStick[nCuntTrigger] | !bTrigger[nCuntTrigger];
		g_bTrigger[nCuntTrigger] = !g_bTrigger[nCuntTrigger];

		g_bStick[nCuntTrigger] = bTrigger[nCuntTrigger];
	}
}

//=====================================================================================================================================================================//
// スティック情報の取得
//=====================================================================================================================================================================//
bool *GetPadPlress(void)
{
	return &g_bPress[0];
}
bool *GetPadTrigger(void)
{
	return &g_bTrigger[0];
}

//=====================================================================================================================================================================//
// 十字キー情報の取得
//=====================================================================================================================================================================//
void CrossKeyPressInfo(DIJOYSTATE2 State)
{
	bool bIsPush[D_CROSS_MAX] = { false,false,false,false };

	switch (State.rgdwPOV[0])
	{
		// 上
	case 0:
		bIsPush[D_CROSS_UP] = true;
		break;
		// 右上
	case 4500:
		bIsPush[D_CROSS_UP] = true;
		bIsPush[D_CROSS_RIGHT] = true;
		break;
		// 右
	case 9000:
		bIsPush[D_CROSS_RIGHT] = true;
		break;
		// 右下
	case 13500:
		bIsPush[D_CROSS_DOWN] = true;
		bIsPush[D_CROSS_RIGHT] = true;
		break;
		// 下
	case 18000:
		bIsPush[D_CROSS_DOWN] = true;
		break;
		// 左下
	case 22500:
		bIsPush[D_CROSS_DOWN] = true;
		bIsPush[D_CROSS_LEFT] = true;
		break;
		// 左
	case 27000:
		bIsPush[D_CROSS_LEFT] = true;
		break;
		// 左上
	case 31500:
		bIsPush[D_CROSS_UP] = true;
		bIsPush[D_CROSS_LEFT] = true;
		break;
	}
	// 情報
	for (int nCuntPress = 0; nCuntPress < JPPRESS_MAX; nCuntPress++)
	{
		g_bPress[nCuntPress] = bIsPush[nCuntPress];
	}
}

void CrossKeyTriggerInfo(DIJOYSTATE2 State)
{
}

//=====================================================================================================================================================================//
// コントローラー情報のポインタ
//=====================================================================================================================================================================//
DIJOYSTATE2 *pGetPadCont(void)
{
	return &g_Controller.State;
}
