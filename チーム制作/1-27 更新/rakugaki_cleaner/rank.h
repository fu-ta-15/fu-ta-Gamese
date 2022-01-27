//=============================================================================
//
// ランク処理 [rank.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

#include "scene2D.h"

//*****************************************************************************
// ランククラス(派生クラス)
//*****************************************************************************
class CRank : public CScene2D
{
public:
	CRank(int nPriority);
	~CRank();

	static CRank *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif