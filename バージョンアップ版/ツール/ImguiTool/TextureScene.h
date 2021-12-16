//*****************************************************************************
//
// テクスチャシーン
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _TEXTURESCENE_H_
#define _TEXTURESCENE_H_
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include <vector>
#include <list>

//-----------------------------------------------------------------------------
// 名前空間による宣言省略
//-----------------------------------------------------------------------------
using namespace std;

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CTextureScene
{
public:
	CTextureScene();
	~CTextureScene();

	void LoadTexture(void);
	void UnLoadTexture(void);
	void ListInTexture(const char *sTexName);
	CTextureScene *TextureListIn(const char *sTexName);

	static void FileDrop(const char *sTexName);

	static int GetListSize(void) { return m_nListSize; }
	static CTextureScene *GetTextureScene(void) { return m_TextureScene.front(); }
	LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }
	char *GetLinkName(void) { return m_sLink; }

private:
	static list<CTextureScene*> m_TextureScene;
	static int				m_nListSize;
	char					*m_sLink;
	LPDIRECT3DTEXTURE9		m_pTexture;
};



#endif // !_TEXTURESCENE_H_


