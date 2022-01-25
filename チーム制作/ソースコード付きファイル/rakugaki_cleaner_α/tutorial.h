//=============================================================================
//
// ロード画面ヘッダー (tutorial.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CURSOR	(TUTORIALUI_MAX - 1)		// 最大カーソル数
#define MAX_ARROW	(2)							// ページ捲り用矢印

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;			// ポリゴン2Dクラス
class CLogo;			// ロゴクラス

//*****************************************************************************
// チュートリアル画面クラス(派生クラス)
//*****************************************************************************
class CTutorial : public CScene
{
public:
	CTutorial(int nPriolity);
	~CTutorial();

	static CTutorial *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool LoadData(void);																// チュートリアルの読み込み
	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);					// ロゴ
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);						// 背景
	bool LoadTutorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName);			// チュートリアルシート
	bool LoadLeftArrow(FILE *pFile, int nNumTex, char **pTextureFileName);				// 左矢印
	bool LoadRightArrow(FILE *pFile, int nNumTex, char **pTextureFileName);				// 右矢印

	bool Logo(FILE *pFile, int nNumTex, char **pTextureFileName);					// ロゴ
	bool TuotorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName);				// 背景

	CScene2D *m_pBg;									// 背景情報のポインタ
	CScene2D **m_pTutorial;								// チュートリアル情報の先頭アドレスを保存するポインタ
	CScene2D *m_pLeftArrow;								// ページ捲り用左矢印情報のポインタ
	CScene2D *m_pRightArrow;							// ページ捲り用右矢印情報のポインタ
	CScene2D **m_pLogo;									// ロゴのポインタ
	int m_nCursor;										// カーソルの位置
	int m_nNumTutorial;									// チュートリアルの枚数
	int m_nNumLogo;										// ロゴの個数

	int m_nCntLogo;										// ロゴのカウント
	int m_nCntTutorial;									// チュートリアルのカウント
};

#endif
