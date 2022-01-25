//=============================================================================
//
// プレイヤーマーカー処理 [player_marker.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PLAYERMARKER_H_
#define _PLAYERMARKER_H_

#include "scene2D.h"

//*****************************************************************************
// プレイヤーマーカークラス(基本クラス)
//*****************************************************************************
class CPlayerMarker : public CScene2D
{
public:
	CPlayerMarker(int nPriority);
	~CPlayerMarker();

	static CPlayerMarker *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
};
#endif