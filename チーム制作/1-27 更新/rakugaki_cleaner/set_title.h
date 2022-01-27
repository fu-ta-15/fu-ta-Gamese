//-------------------------------------------------------------------
//
// タイトル配置ヘッダー (set_title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETTITLE_H_
#define _SETTITLE_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// タイトル配置クラス(基本クラス)
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
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// 背景の配置
	bool LoadTitleLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// タイトルロゴの配置
	bool LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// プレスロゴの配置
};

#endif