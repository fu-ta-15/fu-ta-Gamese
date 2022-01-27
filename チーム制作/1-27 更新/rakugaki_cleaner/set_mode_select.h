//-------------------------------------------------------------------
//
// モード選択画面配置ヘッダー (set_mode_select.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETMODESELECT_H_
#define _SETMODESELECT_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// モード選択画面配置クラス(基本クラス)
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
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// 背景の配置
	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);			// ロゴの配置
};

#endif