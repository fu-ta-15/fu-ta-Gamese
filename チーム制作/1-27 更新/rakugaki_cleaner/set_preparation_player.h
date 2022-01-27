//-------------------------------------------------------------------
//
// プレイヤー待機画面配置ヘッダー (set_preparation_player.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETPREPARATIONPLAYER_H_
#define _SETPREPARATIONPLAYER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// 前方宣言
//-------------------------------------------------------------------
class CScene2D;		// Scene2Dクラス

//-------------------------------------------------------------------
// モード選択画面配置クラス(基本クラス)
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
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// 背景の配置
	bool LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// プレスロゴの配置
	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);			// ロゴの配置
	bool LoadFrame(FILE *pFile, int nNumTex, char **pTextureFileName);			// フレームの配置

	CScene2D *m_pFrame[4];														// フレーム情報保存用ポインタ
	int m_nCntFrame;															// フレームカウント用
};

#endif