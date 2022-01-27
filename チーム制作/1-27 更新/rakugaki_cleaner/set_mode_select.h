//-------------------------------------------------------------------
//
// ���[�h�I����ʔz�u�w�b�_�[ (set_mode_select.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETMODESELECT_H_
#define _SETMODESELECT_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// ���[�h�I����ʔz�u�N���X(��{�N���X)
//-------------------------------------------------------------------
class CSetModeSelect
{
public:
	CSetModeSelect();
	~CSetModeSelect();

	void Init(void);
	void Update(void);
	void LoadData(void);

private:
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// �w�i�̔z�u
	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);			// ���S�̔z�u
};

#endif