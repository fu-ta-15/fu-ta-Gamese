//=============================================================================
//
// �J�E���g�_�E������ [countdown.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;			// �i���o�[�N���X
class CScene2D;			// �V�[��2D�N���X

//*****************************************************************************
// �J�E���g�_�E���N���X(�h���N���X)
//*****************************************************************************
class CCountdown : public CScene
{
public:
	CCountdown(int nPriority);
	~CCountdown();

	static CCountdown *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDigit(int nDigit) { m_nDigit = nDigit; }
	void SetSpace(float fSpace) { m_fSpace = fSpace; }
	void SetTimer(int nTimer) { m_nTimer = nTimer; }
	void SetTexNum(int nTex) { m_nTex = nTex; }

	bool GetFinish(void) { return m_bFinish; }

private:
	bool LoadCountdown(void);												// �J�E���g�_�E�����̓ǂݍ���
	bool LoadTimer(FILE *pFile, int nNumTex, char **pTextureFileName);		// �^�C�}�[���̓ǂݍ���
	bool LoadStartLogo(FILE *pFile, int nNumTex, char **pTextureFileName);	// �J�n���S���̓ǂݍ���
	bool LoadFinishLogo(FILE *pFile, int nNumTex, char **pTextureFileName);	// �I�����S���̓ǂݍ���
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);			// �w�i���̓ǂݍ���
	void UpdateDraw(void);													// �J�E���g�_�E���̕`��X�V

	CNumber *m_apNumber;		// �i���o�[�N���X�̃|�C���^
	CScene2D *m_pStartLogo;		// �Q�[���J�n���S�̃|�C���^
	CScene2D *m_pFinishLogo;	// �Q�[���I�����S�̃|�C���^
	CScene2D *m_pBg;			// �w�i�̃|�C���^

	int m_nDigit;				// ����
	float m_fSpace;				// �z�u�̊Ԋu
	int m_nTimer;				// �^�C�}�[
	int m_nCntInterval;			// �^�C�}�[�����̃C���^�[�o��

	bool m_bFinish;				// �J�E���g�_�E�����I��������ǂ���
	bool m_bTimerFinish;		// �^�C���I�[�o�[���ǂ���
	int m_nTex;					// �e�N�X�`���ԍ�
};
#endif