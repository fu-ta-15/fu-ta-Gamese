//=============================================================================
//
// テクスチャ管理ヘッダー (texture.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// テクスチャ管理(基本クラス)
//*****************************************************************************
class CTexture
{
public:
	// テクスチャの種類
	typedef enum
	{
		TEX_NUMBER = 0,		// 数字
		TEX_SKY_BG,			// 空背景
		TEX_REAF_BG,		// 草背景
		TEX_MAX,
	}TEX;

	CTexture();
	~CTexture();

	HRESULT Load(void);
	void Unload(void);
	void Update(void);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

	int SetTexture(char *pTexFileName);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;

	int m_nNumtextureInfo;						// テクスチャ番号
	int m_nNumFileName;							// ファイルネーム
	char **m_apTexFileName;						// テクスチャファイルネーム
};

#endif