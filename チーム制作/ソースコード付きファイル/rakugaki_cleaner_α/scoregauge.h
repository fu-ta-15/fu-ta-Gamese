//#############################################################################
//
// スコアゲージヘッダファイル [scoregauge.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _SCORE_GAUGE_H_
#define _SCORE_GAUGE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "gauge.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CScoreGauge : public CGauge
{
public:
	CScoreGauge();
	~CScoreGauge();

	static CScoreGauge * Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex);

	HRESULT Init(int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_size;

};


#endif // !_GAUGE_H_
