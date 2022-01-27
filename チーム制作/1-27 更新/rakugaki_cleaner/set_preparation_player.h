//-------------------------------------------------------------------
//
// �v���C���[�ҋ@��ʔz�u�w�b�_�[ (set_preparation_player.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETPREPARATIONPLAYER_H_
#define _SETPREPARATIONPLAYER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// �O���錾
//-------------------------------------------------------------------
class CScene2D;		// Scene2D�N���X

//-------------------------------------------------------------------
// ���[�h�I����ʔz�u�N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetPreparationPlayer
{
public:
	CSetPreparationPlayer();
	~CSetPreparationPlayer();

	void Init(void);
	void Update(void);
	void LoadData(void);
	
	CScene2D *GetFrame(int nCnt) { return m_pFrame[nCnt]; }

private:
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// �w�i�̔z�u
	bool LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// �v���X���S�̔z�u
	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);			// ���S�̔z�u
	bool LoadFrame(FILE *pFile, int nNumTex, char **pTextureFileName);			// �t���[���̔z�u

	CScene2D *m_pFrame[4];														// �t���[�����ۑ��p�|�C���^
	int m_nCntFrame;															// �t���[���J�E���g�p
};

#endif