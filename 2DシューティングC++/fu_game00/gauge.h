//*****************************************************************************
//
// ゲージ処理 [gauge.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "polygon.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CGauge
{
public:
	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CGauge();
	~CGauge();

	static CGauge *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,int Stock);

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Relese(void);

private:
	int			m_nStock;
	D3DXVECTOR3	m_pos;
	D3DXVECTOR3	m_size;
	D3DXCOLOR	m_col;

	CPolygon	*m_pOutPolygon;
	CPolygon	*m_pInPolygon;
};

#endif // !_GAUGE_H_
