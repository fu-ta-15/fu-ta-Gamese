
// テクスチャシーン

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

	static LPDIRECT3DTEXTURE9 GetTextureData(int nID) { return; }

private:
	static CTextureScene	*m_Top;
	static CTextureScene	*m_Cur;
	CTextureScene			*m_Next;
	CTextureScene			*m_Prev;

};



#endif // !_TEXTURESCENE_H_


