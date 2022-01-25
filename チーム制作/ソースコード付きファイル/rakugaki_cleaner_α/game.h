//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : 
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "base.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTimer;			// タイマークラス
class CPlayer_Game;		// プレイヤークラス
class CStage;			// ステージクラス
class CSetGame;			// データ読み込みクラス
class CCountdown;		// カウントダウンクラス
class CGraffitiManager;	// ラクガキ管理クラス
class CGaugeManager;	// ゲージ管理クラス
class CCamera;			// カメラクラス

//*****************************************************************************
// ゲーム画面クラス(派生クラス)
//*****************************************************************************
class CGame : public CBase
{
public:
	// ゲームモード
	typedef enum
	{
		GAMEMODE_NORMAL = 0,			// 通常
		GAMEMODE_FINISH,				// 終了
		GAMEMODE_TRANSITION_MODESELECT,	// モード選択に遷移
		GAMEMODE_TRANSITION_GAME,		// ゲームに遷移
		GAMEMODE_TRANSITION_RESULT,		// リザルトに遷移
		GAMEMODE_MAX,
	}GAMEMODE;

	CGame();
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTimer *GetTimer(void) { return m_pTimer; }
	static void SetTimer(CTimer *pTimer) { m_pTimer = pTimer; }
	static CPlayer_Game **GetPlayer(void) { return m_pPlayer; }
	static CStage *GetStage(void) { return m_pStage; }
	static CCountdown *GetCountdwn(void) { return m_pCountdown; }
	static CGraffitiManager *GetGraffitiManager(void) { return m_pGraffitiManager; }
	static CGaugeManager *GetGaugeManager(void) { return m_pGaugeManager; }
	static int GetnCntPlayer(void) { return m_nCntPlayerOP; }
	static void SetGameMode(GAMEMODE gamemode) { m_gamemode = gamemode; }
	static GAMEMODE GetGameMode(void) { return m_gamemode; }
	static CCamera *GetCamera(void) { return m_pCamera; }

	int GetNumPlayer(void) { return m_nNumPlayer; }

private:
	static CTimer *m_pTimer;						// タイマー情報のポインタ
	static CPlayer_Game **m_pPlayer;				// プレイヤー情報のポインタ
	static CSetGame *m_pLoadData;					// データ読み込み情報のポインタ
	static CStage *m_pStage;						// ステージ情報のポインタ
	static CCountdown *m_pCountdown;				// カウントダウン情報のポインタ
	static CGraffitiManager *m_pGraffitiManager;	// ラクガキ管理情報のポインタ
	static CGaugeManager *m_pGaugeManager;			// ゲージ管理情報のポインタ

	static int m_nCntPlayerOP;
	static GAMEMODE m_gamemode;
	static CCamera *m_pCamera;						// カメラ情報を格納する先頭アドレスのポインタ

	int m_nNumPlayer;
};
#endif