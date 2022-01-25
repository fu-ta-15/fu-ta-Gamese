//=============================================================================
//
// タイマー処理 [timer.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT	(2)	// 桁数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;			// ナンバークラス

//*****************************************************************************
// タイマークラス(派生クラス)
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority);
	~CTimer();

	static CTimer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nDigit, float fSpace, int nTimer, int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDigit(int nDigit) { m_nDigit = nDigit; }
	void SetSpace(float fSpace) { m_fSpace = fSpace; }
	void SetTimer(int nTimer) { m_nTimer = nTimer; }
	void SetTexNum(int nTex) { m_nTex = nTex; }
	int GetTimer(void) { return m_nTimer; }

private:
	void UpdateDraw(void);				// タイマーの描画更新
	void FlashTimer(void);				// タイマーの点滅

	CNumber *m_apNumber[MAX_DIGIT];		// ナンバークラスのポインタ

	int m_nDigit;						// 桁数
	float m_fSpace;						// 配置の間隔
	int m_nTimer;						// タイマー
	int m_nCntInterval;					// タイマー増減のインターバル
	int m_nCntFlash;					// 点滅用カウンタ

	int m_nTex;							// テクスチャ番号
};
#endif