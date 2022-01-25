//=============================================================================
//
// タイトルロゴ処理 [title_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "scene2D.h"

//*****************************************************************************
// タイトルロゴクラス(派生クラス)
//*****************************************************************************
class CTitleLogo : public CScene2D
{
public:
	CTitleLogo(int nPriority);
	~CTitleLogo();

	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nTex;							// テクスチャ番号
};
#endif