//=============================================================================
//
// ラクガキ処理 [graffiti.h]
// Author : 
//
//=============================================================================
#ifndef _GRAFFITI_H_
#define _GRAFFITI_H_

#include "scene3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//-----------------------------------------------------------------------------
// ラクガキクラス(派生クラス)
//-----------------------------------------------------------------------------
class CGraffiti : public CScene3D
{
public:
	// ラクガキの色
	typedef enum
	{
		GRAFFITICOLOR_BLACK = 0,		// 黒
		GRAFFITICOLOR_COLORFUL,			// カラフル(黒以外)
	}GraffitiColor;

	CGraffiti(int nPriority);
	~CGraffiti();

	static CGraffiti *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	GraffitiColor GetGraffitiCol(void) { return m_graffitiCol; }

	bool Collision(D3DXVECTOR3 pos,D3DXVECTOR3 size,int nType);			// ラクガキの当たり判定

private:
	GraffitiColor m_graffitiCol;			// ラクガキの色
};
#endif
