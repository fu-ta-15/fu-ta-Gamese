//##################################################################################################################################################################//
//
// マネージャーヘッダファイル [manager.h]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//
#ifndef _MANAGER_H_
#define _MANAGER_H_

//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define GET_DEVICE		(CManager::GetRenderer()->GetDevice())
#define D3D_DEVICE		(GET_DEVICE)

//-----------------------------------------------------------------------------
//前方宣言
//-----------------------------------------------------------------------------
class CRenderer;	// レンダラー
class CKey;			// キーボード
class CFade;		// フェード
class CScene;		// シーン
class CTitle;		// タイトル
class CTutorial;	// チュートリアル
class CGame;		// ゲーム
class CResult;		// リザルト
class CPause;		// ポーズ
class CSound;		// サウンド

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CManager
{
public:
	//-----------------------------------------------------------------------------
	//モードの列挙型
	//-----------------------------------------------------------------------------
	typedef enum
	{
		MODE_TITLE = 0,	//タイトル画面
		MODE_TUTORIAL,	//チュートリアル画面
		MODE_GAME,		//ゲーム画面
		MODE_RESULT,	//リザルト画面
		MODE_MAX		//MAX数値
	}MODE;

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void UninitMode(MODE mode);
	static void CreateMode(MODE mode);

	// Set関数
	static void SetMode(MODE mode);

	// Get関数
	static MODE GetMode(void);									
	static CRenderer *GetRenderer(void);
	static CKey *GetKey(void);
	static CFade * CManager::GetFade(void){ return m_pFade; }
	static CSound *GetSound(void) { return m_pSound; }
	static CPause *GetPause(void) { return m_pPause; }

private:
	void PauseUpdate(void);

	static MODE				 m_mode;				// モード
	static CRenderer		*m_pRenderer;			// レンダラーポインタ
	static CKey				*m_pKey;				// キーポインタ
	static CFade			*m_pFade;				// フェードクラスのポインタ
	static CTitle			*m_pTitle;				// タイトルクラスのポインタ
	static CTutorial		*m_pTutorial;			// チュートリアルのポインタ
	static CGame			*m_pGame;				// ゲームのポインタ
	static CResult			*m_pResult;				// リザルトのポインタ
	static CPause			*m_pPause;				// ポーズのポインタ
	static CSound			*m_pSound;				// サウンドのポインタ
	static bool				 m_bPause;
};




#endif // !_MANAGER_H_
