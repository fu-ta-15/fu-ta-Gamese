//*****************************************************************************
//
// テクスチャシーン
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#include "main.h"

#ifndef _TEXTURESCENE_H_
#define _TEXTURESCENE_H_

class CTextureScene
{
public:
	CTextureScene();
	~CTextureScene();

	void LoadTexture(void);
	void UnLoadTexture(void);
	void CreateTexture(const char *sTexName);



private:
	CTextureScene			*m_pNext;
	CTextureScene			*m_pPrev;

	char					m_sLink[256];
	int						m_nID;
	LPDIRECT3DTEXTURE9		m_pTexture;

};



#endif // !_TEXTURESCENE_H_


