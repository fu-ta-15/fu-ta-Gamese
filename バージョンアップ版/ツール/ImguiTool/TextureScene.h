//*****************************************************************************
//
// �e�N�X�`���V�[��
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _TEXTURESCENE_H_
#define _TEXTURESCENE_H_
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CTextureScene
{
public:
	CTextureScene();
	~CTextureScene();

	void LoadTexture(void);
	void UnLoadTexture(void);
	void CreateTexture(const char *sTexName);

private:
	static CTextureScene	*m_pTexScene;
	CTextureScene			*m_pNext;
	CTextureScene			*m_pPrev;

	char					*m_sLink;
	int						m_nID;
	LPDIRECT3DTEXTURE9		m_pTexture;

};



#endif // !_TEXTURESCENE_H_


