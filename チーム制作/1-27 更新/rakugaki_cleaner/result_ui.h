//=============================================================================
//
// リザルト画面のUI処理 [result_ui.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTUI_H_
#define _RESULTUI_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;			// スコアクラス
class CNumber;			// ナンバークラス
class CScene2D;			// Cscene2Dクラス

//*****************************************************************************
// リザルト画面クラス(派生クラス)
//*****************************************************************************
class CResult_UI : public CScene
{
public:
	CResult_UI(int nPriority = PRIORITY_UI);
	~CResult_UI();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetAchivement(void) { return m_bAchivement; }

private:
	bool InitDisplayAchivement(void);
	void InitDisplayAchivementPlayer(D3DXVECTOR3 pos, int nCnt);

	CScore *m_pScoreInteger;	// スコアクラス情報のポインタ(整数部分)
	CNumber *m_pNumberDecimal;	// ナンバークラス情報のポインタ(小数部分)
	CScene2D *m_pLogoComma;		// コンマロゴ情報のポインタ
	CScene2D *m_pLogoPercent;	// パーセントロゴ情報のポインタ

	CScore *m_pScoreIntegerPlayer[4];	// スコアクラス情報のポインタ(整数部分)
	CNumber *m_pNumberDecimalPlayer[4];	// ナンバークラス情報のポインタ(小数部分)
	CScene2D *m_pLogoCommaPlayer[4];		// コンマロゴ情報のポインタ
	CScene2D *m_pLogoPercentPlayer[4];	// パーセントロゴ情報のポインタ

	bool m_bAchivement;
};
#endif