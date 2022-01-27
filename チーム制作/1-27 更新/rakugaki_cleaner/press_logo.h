//=============================================================================
//
// �v���X���S���� [press_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PRESSLOGO_H_
#define _PRESSLOGO_H_

#include "scene2D.h"

//*****************************************************************************
// �v���X���S�N���X(�h���N���X)
//*****************************************************************************
class CPressLogo : public CScene2D
{
public:
	CPressLogo(int nPriority);
	~CPressLogo();

	static CPressLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nCntSwap,int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nCntSwap, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetStartCnt(bool bStart);
	bool GetTransition(void) { return m_bTransition; }
	void PressButton(void);				// �{�^�������������ǂ���

private:
	void ChangeColor(void);				// �F�ύX����

	bool m_bCounter;					// �J�E���g���邩�ǂ���
	bool m_bPressButton;				// �{�^�������������ǂ���
	bool m_bTransition;					// �J�ڂ��邩�ǂ���
	int m_nCntInterval;					// �C���^�[�o���J�E���g�p
	int m_nCntSwap;						// �܂�Ԃ��J�E���g

	int m_nTex;							// �e�N�X�`���ԍ�
};
#endif