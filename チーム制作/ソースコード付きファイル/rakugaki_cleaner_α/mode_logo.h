//=============================================================================
//
// モードロゴ処理 [mode_logo.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _MODELOGO_H_
#define _MODELOGO_H_

#include "scene2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// モードロゴクラス(派生クラス)
//*****************************************************************************
class CModeLogo : public CScene
{
public:
	CModeLogo(int nPriority);
	~CModeLogo();

	static CModeLogo *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool LoadModeLogo(void);

private:
	bool ModeLogo(FILE *pFile, int nNumTex, char **pTextureFileName);

	CScene2D **m_pLogo;					// ロゴ情報の先頭アドレスを保存するポインタ
	int *m_nNumMode;					// 何番目のモードを選択しているか保存
	int m_nNumLogo;						// ロゴの総数
	int m_nCntLogo;						// ロゴ数カウント用
	int m_nNumSelect;					// 現在選んでいるロゴの番号
};
#endif