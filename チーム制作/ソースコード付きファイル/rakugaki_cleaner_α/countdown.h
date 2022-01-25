//=============================================================================
//
// カウントダウン処理 [countdown.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;			// ナンバークラス
class CScene2D;			// シーン2Dクラス

//*****************************************************************************
// カウントダウンクラス(派生クラス)
//*****************************************************************************
class CCountdown : public CScene
{
public:
	CCountdown(int nPriority);
	~CCountdown();

	static CCountdown *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDigit(int nDigit) { m_nDigit = nDigit; }
	void SetSpace(float fSpace) { m_fSpace = fSpace; }
	void SetTimer(int nTimer) { m_nTimer = nTimer; }
	void SetTexNum(int nTex) { m_nTex = nTex; }

	bool GetFinish(void) { return m_bFinish; }

private:
	bool LoadCountdown(void);												// カウントダウン情報の読み込み
	bool LoadTimer(FILE *pFile, int nNumTex, char **pTextureFileName);		// タイマー情報の読み込み
	bool LoadStartLogo(FILE *pFile, int nNumTex, char **pTextureFileName);	// 開始ロゴ情報の読み込み
	bool LoadFinishLogo(FILE *pFile, int nNumTex, char **pTextureFileName);	// 終了ロゴ情報の読み込み
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);			// 背景情報の読み込み
	void UpdateDraw(void);													// カウントダウンの描画更新

	CNumber *m_apNumber;		// ナンバークラスのポインタ
	CScene2D *m_pStartLogo;		// ゲーム開始ロゴのポインタ
	CScene2D *m_pFinishLogo;	// ゲーム終了ロゴのポインタ
	CScene2D *m_pBg;			// 背景のポインタ

	int m_nDigit;				// 桁数
	float m_fSpace;				// 配置の間隔
	int m_nTimer;				// タイマー
	int m_nCntInterval;			// タイマー増減のインターバル

	bool m_bFinish;				// カウントダウンが終わったかどうか
	bool m_bTimerFinish;		// タイムオーバーかどうか
	int m_nTex;					// テクスチャ番号
};
#endif