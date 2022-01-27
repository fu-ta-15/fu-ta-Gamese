//=============================================================================
//
// �^�C�}�[���� [timer.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT	(2)	// ����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;			// �i���o�[�N���X

//*****************************************************************************
// �^�C�}�[�N���X(�h���N���X)
//*****************************************************************************
class CTimer : public CScene
{
public:
	CTimer(int nPriority);
	~CTimer();

	static CTimer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nDigit, float fSpace, int nTimer, int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDigit(int nDigit) { m_nDigit = nDigit; }
	void SetSpace(float fSpace) { m_fSpace = fSpace; }
	void SetTimer(int nTimer) { m_nTimer = nTimer; }
	void SetTexNum(int nTex) { m_nTex = nTex; }
	int GetTimer(void) { return m_nTimer; }

private:
	void UpdateDraw(void);				// �^�C�}�[�̕`��X�V
	void FlashTimer(void);				// �^�C�}�[�̓_��

	CNumber *m_apNumber[MAX_DIGIT];		// �i���o�[�N���X�̃|�C���^

	int m_nDigit;						// ����
	float m_fSpace;						// �z�u�̊Ԋu
	int m_nTimer;						// �^�C�}�[
	int m_nCntInterval;					// �^�C�}�[�����̃C���^�[�o��
	int m_nCntFlash;					// �_�ŗp�J�E���^

	int m_nTex;							// �e�N�X�`���ԍ�
};
#endif