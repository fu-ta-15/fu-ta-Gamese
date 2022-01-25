//=============================================================================
//
// プレイヤー準備画面処理 [preparation_player.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PREPARATIONPLAYER_H_
#define _PREPARATIONPLAYER_H_

#include "base.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPreparationPlayerManager;		// プレイヤー準備画面管理クラス

//*****************************************************************************
// プレイヤー待機画面クラス(派生クラス)
//*****************************************************************************
class CPreparationPlayer : public CBase
{
public:
	// タイトルのモード
	typedef enum
	{
		PREPARATIONPLAYER_NORMAL = 0,				// 通常
		PREPARATIONPLAYER_TRANSITION_GAME_BATTLE,	// シングルモード遷移
		PREPARATIONPLAYER_TRANSITION_GAME_COOPERATION,	// マルチモード遷移
		PREPARATIONPLAYER_TRANSITION_MODESELECT,	// モード選択
		PREPARATIONPLAYER_MAX,
	}PREPARATIONPLAYER;

	CPreparationPlayer();
	~CPreparationPlayer();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	PREPARATIONPLAYER GetMode(void) { return m_preparationPlayer; }
	void SetMode(PREPARATIONPLAYER preparationPlayer) { m_preparationPlayer = preparationPlayer; }

	static CPreparationPlayerManager *GetPPM(void) { return m_pPPM; }
	static int GetnCntPlayer(void) { return m_nCntPlayerOP; }

private:
	static int m_nCntPlayerOP;
	int m_nNumPlayer;
	PREPARATIONPLAYER m_preparationPlayer;		// モード選択モード
	static CPreparationPlayerManager *m_pPPM;	// プレイヤー準備画面管理情報のポイン
};
#endif