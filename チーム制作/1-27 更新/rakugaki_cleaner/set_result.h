//-------------------------------------------------------------------
//
// リザルト配置ヘッダー (set_title.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETRESULT_H_
#define _SETRESULT_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// リザルト配置クラス(基本クラス)
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
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// 背景の配置
};

#endif