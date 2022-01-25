//=============================================================================
//
// リザルト画面管理処理 [result_manager.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;			// ポリゴン2Dクラス

//*****************************************************************************
// リザルト画面管理クラス(派生クラス)
//*****************************************************************************
class CResultManager : public CScene
{
public:
	CResultManager(int nPriority = PRIORITY_UI);
	~CResultManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	bool LoadData(void);

	bool LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// その他ロゴ
	bool LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName);			// 背景
	bool LoadPauseMenu(FILE *pFile, int nNumTex, char **pTextureFileName);	// ポーズメニュー

	bool Bg(FILE *pFile, int nNumTex, char **pTextureFileName);				// 背景
	bool MenuBg(FILE *pFile, int nNumTex, char **pTextureFileName);			// メニュー背景
	bool MenuLogo(FILE *pFile, int nNumTex, char **pTextureFileName);		// メニューロゴ
	bool MenuLogoBg(FILE *pFile, int nNumTex, char **pTextureFileName);		// メニューロゴ背景

	CScene2D **m_pBg;					// 背景情報
	CScene2D **m_pPauseMenuBg;			// ポーズメニュー背景情報の先頭アドレスを保存するポインタ
	CScene2D **m_pPauseMenuLogo;		// ポーズメニューロゴ情報の先頭アドレスを保存するポインタ
	CScene2D **m_pPauseMenuLogoBg;		// ポーズメニューロゴ背景情報の先頭アドレスを保存するポインタ
	CScene2D **m_pLogo;					// その他ロゴ情報の先頭アドレスを保存するポインタ
	int m_nPauseSelect;					// ポーズメニューから遷移する場所を保存

	int m_nNumBg;						// 背景数
	int m_nNumMenuBg;					// ポーズメニュー背景数
	int m_nNumLogo;						// ロゴ数
	int m_nNumLogoBg;					// ロゴ背景
	int m_nLogo;						// その他ロゴ

	int m_nCntBg;
	int m_nCntMenuBg;
	int m_nCntMenuLogo;
	int m_nCntMenuLogoBg;
	int m_nCntLogo;

	bool m_bOK;

	int m_nCounter;
	CScene2D *m_pLogoNext;
};
#endif