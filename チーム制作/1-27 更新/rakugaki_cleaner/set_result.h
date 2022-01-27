//-------------------------------------------------------------------
//
// ���U���g�z�u�w�b�_�[ (set_title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETRESULT_H_
#define _SETRESULT_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// ���U���g�z�u�N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetResult
{
public:
	CSetResult();
	~CSetResult();

	void Init(void);
	void Update(void);
	void LoadData(void);

private:
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// �w�i�̔z�u
};

#endif