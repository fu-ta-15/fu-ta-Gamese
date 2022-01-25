//-------------------------------------------------------------------
//
// �f�[�^�ǂݍ��݃w�b�_�[ (set_game.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _LOADDATA_H_
#define _LOADDATA_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// �f�[�^�ǂݍ��݃N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetGame
{
public:
	CSetGame();
	~CSetGame();

	void Init(void);
	void Update(void);

private:
	bool LoadFloor(void);														// �����̓ǂݍ���
	bool LoadUIInfo(void);														// UI���̓ǂݍ���

	bool LoadSkillGauge(FILE *pFile, int nNumTex, char **pTextureFileName);		// �X�L���Q�[�W
	bool LoadScoreGauge(FILE *pFile, int nNumTex, char **pTextureFileName);		// �X�R�A�Q�[�W
	bool LoadGraffitiGauge(FILE *pFile, int nNumTex, char **pTextureFileName);	// ���N�K�L�Q�[�W
	bool LoadGaugeFrame(FILE *pFile, int nNumTex, char **pTextureFileName);		// �Q�[�W�̘g
	bool LoadPlayerUI(FILE *pFile,int nNumTex, char **pTextureFileName);		// �v���C���[UI
	bool LoadEraserStateUI(FILE *pFile, int nNumTex, char **pTextureFileName);	// �����S���̏��UI

	bool LoadTimer(FILE *pFile,int nNumTex, char **pTextureFileName);			// �^�C�}�[

	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// �e��w�i

	D3DXVECTOR3 m_pos;		// �ʒu�ۑ��p
	D3DXVECTOR2 m_size;		// �T�C�Y�ۑ��p
	int m_nNumTex;			// �e�N�X�`���ԍ��ۑ��p

	int m_nCntScoreGauge;
	int m_nCntSkillGauge;
};

#endif