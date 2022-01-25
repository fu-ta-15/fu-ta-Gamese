//-------------------------------------------------------------------
//
// �^�C�g���z�u�w�b�_�[ (set_title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETTITLE_H_
#define _SETTITLE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// �^�C�g���z�u�N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetTitle
{
public:
	CSetTitle();
	~CSetTitle();

	void Init(void);
	void Update(void);
	void LoadData(void);

private:
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// �w�i�̔z�u
	bool LoadTitleLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// �^�C�g�����S�̔z�u
	bool LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// �v���X���S�̔z�u
};

#endif