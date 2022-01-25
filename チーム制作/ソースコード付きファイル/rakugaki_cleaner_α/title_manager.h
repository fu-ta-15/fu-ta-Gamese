//=============================================================================
//
// タイトル画面管理処理 [title_manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPressLogo;		// プレスロゴクラス

//*****************************************************************************
// タイトル画面管理クラス(派生クラス)
//*****************************************************************************
class CTitleManager : public CScene
{
public:
	CTitleManager();
	~CTitleManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPressLogo(CPressLogo *pPressLogo) { m_pPressLogo = pPressLogo; }

private:
	CPressLogo *m_pPressLogo;		// プレスロゴ情報のポインタ
	bool m_bPress;					// 押したかどうか
};
#endif