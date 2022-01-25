//#############################################################################
//
// ゲージヘッダファイル [gauge.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _GAUGE_H_
#define _GAUGE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "Scene2D.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CGauge : public CScene2D
{
public:

	enum ADD_GAUGE
	{
		ADD_UP = 0, 
		ADD_DOWN,
		ADD_RIGHT,
		ADD_LEFT,
		ADD_MAX

	};

	CGauge();
	~CGauge();

	static CGauge *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex);

	HRESULT Init(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetValue(float fPercent);
	void AddGauge(ADD_GAUGE type,int nVtxNo1,int nVtxNo2);
	D3DXVECTOR3 GetPosDef(void) { return m_posDef; }
	D3DXVECTOR2 GetSizeDef(void) { return m_sizeDef; }

private:
	LPDIRECT3DTEXTURE9 m_pTexture = NULL;		// テクスチャ情報のポインタ
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_size;
	D3DXCOLOR	m_col;
	D3DXVECTOR3 m_posDef;
	D3DXVECTOR2 m_sizeDef;
	float m_fMaxNum;
	D3DXVECTOR2 m_AddSize;
	float m_fValue;
};

#endif // !_GAUGE_H_
