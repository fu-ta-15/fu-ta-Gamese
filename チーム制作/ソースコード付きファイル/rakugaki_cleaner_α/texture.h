//=============================================================================
//
// �e�N�X�`���Ǘ��w�b�_�[ (texture.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// �e�N�X�`���Ǘ�(��{�N���X)
//*****************************************************************************
class CTexture
{
public:
	// �e�N�X�`���̎��
	typedef enum
	{
		TEX_NUMBER = 0,		// ����
		TEX_SKY_BG,			// ��w�i
		TEX_REAF_BG,		// ���w�i
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

	int m_nNumtextureInfo;						// �e�N�X�`���ԍ�
	int m_nNumFileName;							// �t�@�C���l�[��
	char **m_apTexFileName;						// �e�N�X�`���t�@�C���l�[��
};

#endif