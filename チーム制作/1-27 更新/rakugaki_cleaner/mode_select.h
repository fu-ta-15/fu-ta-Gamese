//=============================================================================
//
// モード選択画面処理 [mode_select.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

#include "base.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUIBg;		// UI背景クラス
class CModel;		// モデルクラス

//*****************************************************************************
// モード選択画面クラス(派生クラス)
//*****************************************************************************
class CModeSelect : public CBase
{
public:
	// タイトルのモード
	typedef enum
	{
		MODESELECT_NORMAL = 0,					// 通常
		MODESELECT_TRANSITION_GAME_BATTLE,		// シングルモード遷移
		MODESELECT_TRANSITION_GAME_COOPERATION,		// マルチモード遷移
		MODESELECT_TRANSITION_GAME_TUTORIAL,	// チュートリアルモード遷移
		MODESELECT_MAX,
	}MODESELECT;

	CModeSelect();
	~CModeSelect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	MODESELECT GetModeSelect(void) { return m_modeselect; }
	void SetMode(MODESELECT modeselect) { m_modeselect = modeselect; }

private:
	MODESELECT m_modeselect;		// モード選択モード
};
#endif