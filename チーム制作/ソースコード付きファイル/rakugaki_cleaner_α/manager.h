//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : 
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

#include <stdlib.h>
#include <time.h>

// マクロ定義
#define GET_KEYBOARD		(CManager::GetInputKeyboard())		// キーボード情報の取得
#define GET_JOYPAD			(CManager::GetInputJoypad())		// キーボード情報の取得
#define GET_MOUSE			(CManager::GetInputMouse())		// キーボード情報の取得

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;			// レンダラクラス
class CInputKeyboard;		// キーボードクラス
class CInputJoypad;			// ジョイパッドクラス
class CInputMouse;			// マウスクラス
class CSound;				// サウンドクラス
class CFade;				// フェードクラス
class CCamera;				// カメラクラス
class CLight;				// ライトクラス
class CTexture;				// テクスチャクラス
class CBase;				// ベースクラス
class CKeepObject;			// データ保存用クラス

//-----------------------------------------------------------------------------
// マネージャークラス(基本クラス)
//-----------------------------------------------------------------------------
class CManager
{
public:
	// モードの種類
	typedef enum
	{
		MODE_TITLE = 0,		// タイトル
		MODE_MODESELECT,	// モード選択
		MODE_PREPARATION,	// 準備
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_RANKING,		// ランキング
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetMode(CBase *pBase);
	static MODE GetMode(void) { return m_mode; }
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }
	static CFade *GetFade(void) { return m_pFade; }
	static CSound *GetSound(void) { return m_pSound; }
	static CLight *GetLight(void) { return m_pLight; }
	static CTexture *GetTexture(void) { return m_pTexture; }
	static CBase *GetBase(void) { return m_pBase; }
	static CKeepObject *GetKeepObject(void) { return m_pKeepObject; }

	static bool GetPause(void) { return m_bPause; }
	static void SetPause(bool bPause) { m_bPause = bPause; }

private:
	static CRenderer *m_pRenderer;				// レンダラ情報のポインタ
	static CInputKeyboard *m_pInputKeyboard;	// キーボード情報のポインタ
	static CInputJoypad *m_pInputJoypad;		// ジョイパッド情報のポインタ
	static CInputMouse *m_pInputMouse;			// マウス情報のポインタ
	static CSound *m_pSound;					// サウンド情報のポインタ
	static CBase *m_pBase;						// ベースシーン情報のポインタ
	static MODE m_mode;							// モード情報
	static CFade *m_pFade;						// フェード情報のポインタ
	static bool m_bPause;						// ポーズするかどうか
	static CCamera *m_pCamera;					// カメラ情報のポインタ
	static CLight *m_pLight;					// ライト情報のポインタ
	static CTexture *m_pTexture;				// テクスチャ情報のポインタ
	static CKeepObject *m_pKeepObject;			// データ保存用情報のポインタ

	CBase *SetMode(void);
};
#endif