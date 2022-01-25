//=============================================================================
//
// ステージ選択画面処理 [stage_select.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _STAGESELECT_H_
#define _STAGESELECT_H_

#include "base.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// モード選択画面クラス(派生クラス)
//*****************************************************************************
class CStageSelect : public CBase
{
public:
	// タイトルのモード
	typedef enum
	{
		MODESELECT_NORMAL = 0,					// 通常
		MODESELECT_TRANSITION_GAME_SINGLE,		// シングルモード遷移
		MODESELECT_TRANSITION_GAME_MULTI,		// マルチモード遷移
		MODESELECT_MAX,
	}STAGESELECT;

	CStageSelect();
	~CStageSelect();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	STAGESELECT GetStageSelect(void) { return m_stageselect; }
	void SetMode(STAGESELECT stageselect) { m_stageselect = stageselect; }

private:
	STAGESELECT m_stageselect;		// モード選択モード
};
#endif