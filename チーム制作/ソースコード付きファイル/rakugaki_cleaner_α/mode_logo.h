//=============================================================================
//
// ���[�h���S���� [mode_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MODELOGO_H_
#define _MODELOGO_H_

#include "scene2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// ���[�h���S�N���X(�h���N���X)
//*****************************************************************************
class CModeLogo : public CScene
{
public:
	CModeLogo(int nPriority);
	~CModeLogo();

	static CModeLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool LoadModeLogo(void);

private:
	bool ModeLogo(FILE *pFile, int nNumTex, char **pTextureFileName);

	CScene2D **m_pLogo;					// ���S���̐擪�A�h���X��ۑ�����|�C���^
	int *m_nNumMode;					// ���Ԗڂ̃��[�h��I�����Ă��邩�ۑ�
	int m_nNumLogo;						// ���S�̑���
	int m_nCntLogo;						// ���S���J�E���g�p
	int m_nNumSelect;					// ���ݑI��ł��郍�S�̔ԍ�
};
#endif