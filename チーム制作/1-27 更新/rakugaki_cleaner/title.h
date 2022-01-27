//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "base.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CTitleManager;		// タイトル画面管理クラス

//*****************************************************************************
// タイトル画面クラス(派生クラス)
//*****************************************************************************
class CTitle : public CBase
{
public:
	// タイトルのモード
	typedef enum
	{
		TITLEMODE_NORMAL = 0,		// 通常
		TITLEMODE_TRANSITION,		// 遷移
		TITLEMODE_MAX,
	}TITLEMODE;

	CTitle();
	~CTitle();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	TITLEMODE GetTitleMode(void) { return m_titlemode; }
	void SetTitleMode(TITLEMODE titlemode) { m_titlemode = titlemode; }

	static CTitleManager *GetTitleManager(void) { return m_pTitleManager; }

private:
	TITLEMODE m_titlemode;					// タイトルモード
	static CTitleManager *m_pTitleManager;	// タイトル管理情報のポインタ
};
#endif