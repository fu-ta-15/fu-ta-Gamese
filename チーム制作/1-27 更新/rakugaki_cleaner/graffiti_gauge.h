//=============================================================================
//
// ラクガキ用ゲージ処理 [graffiti_gauge.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _GRAFFITI_GAUGE_H_
#define _GRAFFITI_GAUGE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "gauge.h"

//-----------------------------------------------------------------------------
// 前方宣言
//-----------------------------------------------------------------------------
class CScore;			// スコアクラス
class CNumber;			// ナンバークラス
class CScene2D;			// Cscene2Dクラス

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CGraffitiGauge : public CGauge
{
public:
	CGraffitiGauge();
	~CGraffitiGauge();

	static CGraffitiGauge * Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex);

	HRESULT Init(int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRatio(float fRatio) { m_fRatio = fRatio; }
	void SetPosXRightSide(float fPos) { m_fPosXRightSide = fPos; }

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_size;
	float m_fRatio;				// 割合
	float m_fPosXRightSide;		// 右端のX座標

	CScore *m_pScoreInteger;	// スコアクラス情報のポインタ(整数部分)
	CNumber *m_pNumberDecimal;	// ナンバークラス情報のポインタ(小数部分)
	CScene2D *m_pLogoComma;		// コンマロゴ情報のポインタ
	CScene2D *m_pLogoPercent;	// パーセントロゴ情報のポインタ
};
#endif