//=============================================================================
//
// 背景処理 [bg.h]
// Author : 
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "scene2D.h"

//-----------------------------------------------------------------------------
// 背景クラス(基本クラス)
//-----------------------------------------------------------------------------
class CBg : public CScene2D
{
public:
	CBg(int nPriority);
	~CBg();

	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXCOLOR col, D3DXVECTOR2 moveTex,int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, D3DXVECTOR2 moveTex, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void MoveTex(void);		// テクスチャ座標移動

	D3DXVECTOR2 m_tex;		// テクスチャ座標
	D3DXVECTOR2 m_moveTex;	// テクスチャ座標増減用
	int m_nTex;				// テクスチャ番号
};
#endif