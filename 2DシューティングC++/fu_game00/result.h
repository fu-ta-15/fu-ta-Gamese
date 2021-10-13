//*****************************************************************************
//
// リザルト処理 [result.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _RESULT_H_
#define	_RESULT_H_

//=============================================================================
//　インクルードファイル
//=============================================================================
#include "scene.h"
#include "scene2D.h"




class CResult : public CScene
{
public:
	typedef enum Result
	{
		RESULT_UI_BG = 0,
		RESULT_UI_LOGO,
		RESULT_UI_MAX
	}Result;

	CResult();
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void GameEndResult(bool bEnd) { m_bEndGame = bEnd; }

private:
	CScene2D *m_ResultUI[RESULT_UI_MAX];
	static bool m_bEndGame;
	D3DXVECTOR3 m_pos;

};




#endif // !1
