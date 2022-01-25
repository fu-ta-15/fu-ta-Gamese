//-------------------------------------------------------------------
//
// データ読み込みヘッダー (set_game.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _LOADDATA_H_
#define _LOADDATA_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "main.h"

//-------------------------------------------------------------------
// データ読み込みクラス(基本クラス)
//-------------------------------------------------------------------
class CSetGame
{
public:
	CSetGame();
	~CSetGame();

	void Init(void);
	void Update(void);

private:
	bool LoadFloor(void);														// 床情報の読み込み
	bool LoadUIInfo(void);														// UI情報の読み込み

	bool LoadSkillGauge(FILE *pFile, int nNumTex, char **pTextureFileName);		// スキルゲージ
	bool LoadScoreGauge(FILE *pFile, int nNumTex, char **pTextureFileName);		// スコアゲージ
	bool LoadGraffitiGauge(FILE *pFile, int nNumTex, char **pTextureFileName);	// ラクガキゲージ
	bool LoadGaugeFrame(FILE *pFile, int nNumTex, char **pTextureFileName);		// ゲージの枠
	bool LoadPlayerUI(FILE *pFile,int nNumTex, char **pTextureFileName);		// プレイヤーUI
	bool LoadEraserStateUI(FILE *pFile, int nNumTex, char **pTextureFileName);	// 消しゴムの状態UI

	bool LoadTimer(FILE *pFile,int nNumTex, char **pTextureFileName);			// タイマー

	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);				// 各種背景

	D3DXVECTOR3 m_pos;		// 位置保存用
	D3DXVECTOR2 m_size;		// サイズ保存用
	int m_nNumTex;			// テクスチャ番号保存用

	int m_nCntScoreGauge;
	int m_nCntSkillGauge;
};

#endif