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
#include <vector>
#include <list>

//-----------------------------------------------------------------------------
// ���O��Ԃɂ��錾�ȗ�
//-----------------------------------------------------------------------------
using namespace std;

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
	CTextureScene *TextureListIn(const char *sTexName);

	static void FileDrop(const char *sTexName);

	static CTextureScene *GetTexScene(int nID);
	static CTextureScene *GetTextureScene(void) { return m_TextureScene.front(); }
	static int GetListSize(void)				{ return m_nListSize; }
	LPDIRECT3DTEXTURE9 GetTexture(void)			{ return m_pTexture; }
	char *GetLinkName(void)						{ return m_sLink; }

private:
	static list<CTextureScene*> m_TextureScene;		// �e�N�X�`���V�[���̃��X�g
	static int					m_nListSize;		// ���X�g�̃T�C�Y
	char						*m_sLink;			// �e�N�X�`���̃����N
	LPDIRECT3DTEXTURE9			m_pTexture;			// �e�N�X�`��
};



#endif // !_TEXTURESCENE_H_


