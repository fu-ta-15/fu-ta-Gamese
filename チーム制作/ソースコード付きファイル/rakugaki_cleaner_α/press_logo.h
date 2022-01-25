//=============================================================================
//
// プレスロゴ処理 [press_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PRESSLOGO_H_
#define _PRESSLOGO_H_

#include "scene2D.h"

//*****************************************************************************
// プレスロゴクラス(派生クラス)
//*****************************************************************************
class CPressLogo : public CScene2D
{
public:
	CPressLogo(int nPriority);
	~CPressLogo();

	static CPressLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nCntSwap,int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntSwap, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetStartCnt(bool bStart);
	bool GetTransition(void) { return m_bTransition; }
	void PressButton(void);				// ボタンを押したかどうか

private:
	void ChangeColor(void);				// 色変更処理

	bool m_bCounter;					// カウントするかどうか
	bool m_bPressButton;				// ボタンを押したかどうか
	bool m_bTransition;					// 遷移するかどうか
	int m_nCntInterval;					// インターバルカウント用
	int m_nCntSwap;						// 折り返しカウント

	int m_nTex;							// テクスチャ番号
};
#endif