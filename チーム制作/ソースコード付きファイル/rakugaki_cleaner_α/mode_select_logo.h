//=============================================================================
//
// ���[�h�I�����S���� [mode_select_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MODESELECTLOGO_H_
#define _MODESELECTLOGO_H_

#include "scene2D.h"

//*****************************************************************************
// ���[�h�I�����S�N���X(�h���N���X)
//*****************************************************************************
class CModeSelectLogo : public CScene2D
{
public:
	CModeSelectLogo(int nPriority);
	~CModeSelectLogo();

	static CModeSelectLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nTex;							// �e�N�X�`���ԍ�
};
#endif