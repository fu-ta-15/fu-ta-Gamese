//=============================================================================
//
// 影用のモデル [shadowS.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _SHADOWS_H_
#define _SHADOWS_H_

#include "sceneX.h"

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CShadowS : public CSceneX
{
public:

	CShadowS(int nPriority);
	~CShadowS();

	static CShadowS *Create(D3DXVECTOR3 pos,D3DXVECTOR2 size, D3DXVECTOR3 rot, char *pModelFileName);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;
};

#endif
