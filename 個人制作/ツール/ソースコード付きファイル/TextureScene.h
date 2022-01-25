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
	CTextureScene *TextureListIn(const char *sTexName);

	static void FileDrop(const char *sTexName);

	static CTextureScene *GetTexScene(int nID);
	static CTextureScene *GetTextureScene(void) { return m_TextureScene.front(); }
	static int GetListSize(void)				{ return m_nListSize; }
	LPDIRECT3DTEXTURE9 GetTexture(void)			{ return m_pTexture; }
	char *GetLinkName(void)						{ return m_sLink; }

private:
	static list<CTextureScene*> m_TextureScene;		// テクスチャシーンのリスト
	static int					m_nListSize;		// リストのサイズ
	char						*m_sLink;			// テクスチャのリンク
	LPDIRECT3DTEXTURE9			m_pTexture;			// テクスチャ
};



#endif // !_TEXTURESCENE_H_


